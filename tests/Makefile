example_instr: example.cpp
	clang++ example.cpp -O -emit-llvm -S
	opt.sh example.ll -o example.instr.ll -S
	clang++ example.instr.ll -L${INSTRUMENTATION_LIB_DIR} -lprobe -Wl,-rpath=${INSTRUMENTATION_LIB_DIR} -o example_instr
clean:
	rm *.ll example_instr
