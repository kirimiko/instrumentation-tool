#include "probe.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include <llvm/IR/Constant.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Type.h>
#include <unordered_map>

using namespace llvm;
using std::string;
using std::vector;
class InstrumentationPass : public llvm::PassInfoMixin<InstrumentationPass>
{
public:
    llvm::PreservedAnalyses run(llvm::Function &F, llvm::FunctionAnalysisManager &FAM)
    {
        auto &M = *F.getParent();
        auto &C = F.getContext();
        auto probe_type = FunctionType::get(Type::getVoidTy(C), {Type::getInt64Ty(C)}, false);
        std::unordered_map<llvm::Type *, llvm::Function *> probe_map;

        auto function_creator = [&](Type *type, const string &name) {
            // vector<Type *> func_params = vector<Type *>{type, Type::getInt32Ty(C), Type::getInt32Ty(C)};
            if (auto res = M.getFunction(name))
            {
                return res;
            }
            else
            {
                // assert(false);
                vector<Type *> func_params = vector<Type *>{Type::getInt64Ty(C), type, Type::getInt32Ty(C)};
                FunctionType *func_type = FunctionType::get(Type::getVoidTy(C), func_params, false);
                return Function::Create(func_type, GlobalValue::LinkageTypes::ExternalLinkage, name, F.getParent());
            }
        };
        probe_map.emplace(Type::getDoubleTy(C),
                          function_creator(Type::getDoubleTy(C), INSTRUMENTATION_PREFIX_STR PROBE_PREFIX "double"));
        probe_map.emplace(Type::getFloatTy(C),
                          function_creator(Type::getFloatTy(C), INSTRUMENTATION_PREFIX_STR PROBE_PREFIX "float"));
        probe_map.emplace(Type::getInt32Ty(C),
                          function_creator(Type::getInt32Ty(C), INSTRUMENTATION_PREFIX_STR PROBE_PREFIX "int32_t"));
        probe_map.emplace(Type::getInt64Ty(C),
                          function_creator(Type::getInt64Ty(C), INSTRUMENTATION_PREFIX_STR PROBE_PREFIX "int64_t"));
        probe_map.emplace(Type::getInt8Ty(C),
                          function_creator(Type::getInt8Ty(C), INSTRUMENTATION_PREFIX_STR PROBE_PREFIX "int8_t"));
        probe_map.emplace(Type::getInt16Ty(C),
                          function_creator(Type::getInt16Ty(C), INSTRUMENTATION_PREFIX_STR PROBE_PREFIX "int16_t"));
        probe_map.emplace(Type::getInt1Ty(C),
                          function_creator(Type::getInt1Ty(C), INSTRUMENTATION_PREFIX_STR PROBE_PREFIX "int1_t"));
        for (auto &bb : F)
        {
            for (auto &instr : bb)
            {
                if (auto load = dyn_cast<LoadInst>(&instr))
                {
                    auto arg =
                        BitCastInst::CreateBitOrPointerCast(load->getPointerOperand(), Type::getInt64Ty(C), "", load);
                    auto probe_func = probe_map[load->getType()];
                    if (!probe_func)
                    {
                        continue;
                    }
                    auto call = CallInst::Create(probe_func, {arg, load, ConstantInt::get(Type::getInt32Ty(C), 0)}, "");
                    call->insertAfter(load);
                }
                if (auto store = dyn_cast<StoreInst>(&instr))
                {
                    auto arg =
                        BitCastInst::CreateBitOrPointerCast(store->getPointerOperand(), Type::getInt64Ty(C), "", store);
                    auto probe_func = probe_map[store->getValueOperand()->getType()];
                    if (!probe_func)
                    {
                        continue;
                    }
                    auto call = CallInst::Create(probe_func,
                                                 {
                                                     arg,
                                                     store->getValueOperand(),
                                                     ConstantInt::get(Type::getInt32Ty(C), 1),
                                                 },
                                                 "",
                                                 store);
                }
            }
        }
        return llvm::PreservedAnalyses::none();
    }
};

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo()
{
    return {LLVM_PLUGIN_API_VERSION, "instrumentation", LLVM_VERSION_STRING, [](PassBuilder &PB) {
                PB.registerPipelineParsingCallback(
                    [](StringRef Name, llvm::FunctionPassManager &PM, ArrayRef<llvm::PassBuilder::PipelineElement>) {
                        if (Name == "instrumentation")
                        {
                            PM.addPass(InstrumentationPass());
                            return true;
                        }
                        return false;
                    });
                PB.registerVectorizerStartEPCallback([](llvm::FunctionPassManager &FPM, llvm::OptimizationLevel O) {
                    FPM.addPass(InstrumentationPass());
                });
            }};
}