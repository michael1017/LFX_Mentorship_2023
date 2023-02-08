TEST_OUT_DIR="test_out"

make clean_test
make -j EXTRAFLAGS="-fprofile-instr-generate -fcoverage-mapping" test

LLVM_PROFILE_FILE=$TEST_OUT_DIR/test.profraw ./x_testall
llvm-profdata merge -sparse $TEST_OUT_DIR/test.profraw -o $TEST_OUT_DIR/test.profdata
llvm-cov report ./x_testall -instr-profile=$TEST_OUT_DIR/test.profdata
