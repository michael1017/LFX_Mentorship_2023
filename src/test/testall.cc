#include <gtest/gtest.h>

int main(int argc, char **argv) {
    // disable stderr output in test mode
    fclose(stderr); 

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}