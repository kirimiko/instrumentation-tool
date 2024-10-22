# 访存插桩工具
该工具可以在编译过程中为应用程序插桩，采集所有访存的指针与值
## Dependencies
llvm 17.0.0

## Build and Install

```
export LLVM_INSTALL_PREFIX=/path/to/llvm
cmake -B build --preset release
cmake --build build
```

加载环境变量

```
cp env.template env.sh
# modify INSTRUMENTATION_LIB_DIR to /path/to/lib ($PROJECT_PATH/build)
source env.sh
```

## How to use this tool
tests下有使用样例，make后即可得到插桩的产物example_instr，运行后输出示例如下
```
...
prt:  94350487780992
prt:  94350487780996
prt:  94350487781000
prt:  94350487781004
prt:  94350487781008
prt:  94350487781012
prt:  94350487781016
prt:  94350487781020
prt:  94350487781024
prt:  94350487781028
prt:  94350487781032
prt:  94350487781036
sum: 3.58365e+10prt:  140500907824056
prt:  140500907853624
prt:  140500907853635
```

使用该工具编译其它工具时，注意需要添加-O选项，如果为-O0则pass不会被运行

