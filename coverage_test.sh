make EXTRAFLAGS="-fprofile-instr-generate -fcoverage-mapping" test
LLVM_PROFILE_FILE="test.profraw" ./x_testall
llvm-profdata merge -sparse test.profraw -o test.profdata
llvm-cov report ./x_testall -instr-profile=test.profdata
