extern "C" {
    #include "baseutil.h"

    #include "option.h"
    #include "argparse.h"
}
#include <stdbool.h>
#include <gtest/gtest.h>

TEST(baseutil, is_null_ptr) {
    const Option* opt_null = NULL;
    const Option* opt = create_Option(NULL, -1);
    const ParseData* pd_null = NULL;
    const ParseData* pd = create_ParseData();
    const char* c_null = NULL;
    const char c = 'c';

    const char func_name[32] = "isNullPtrTest";
    EXPECT_TRUE(is_null_ptr(opt_null, func_name));
    EXPECT_TRUE(is_null_ptr(pd_null, func_name));
    EXPECT_TRUE(is_null_ptr(c_null, func_name));
    EXPECT_FALSE(is_null_ptr(opt, func_name));
    EXPECT_FALSE(is_null_ptr(pd, func_name));
    EXPECT_FALSE(is_null_ptr(&c, func_name));

    delete_Option((Option* )opt);
    delete_ParseData((ParseData* )pd);
}

TEST(baseutil, is_file_exist) {
    EXPECT_FALSE(is_file_exist(NULL));
    EXPECT_FALSE(is_file_exist("/dev/null/foo"));
    EXPECT_TRUE (is_file_exist("/dev/null"));
}

TEST(baseutil, is_valid_digit_string) {
    EXPECT_FALSE(is_valid_digit_string(NULL));
    EXPECT_FALSE(is_valid_digit_string("\0"));
    EXPECT_FALSE(is_valid_digit_string("023-"));
    EXPECT_FALSE(is_valid_digit_string("aA"));
    EXPECT_FALSE(is_valid_digit_string("%"));
    EXPECT_FALSE(is_valid_digit_string("-2147483649"));
    EXPECT_FALSE(is_valid_digit_string("2147483648"));
    
    EXPECT_TRUE (is_valid_digit_string("+"));
    EXPECT_TRUE (is_valid_digit_string("-"));
    EXPECT_TRUE (is_valid_digit_string("+000100"));
    EXPECT_TRUE (is_valid_digit_string("-0123456789"));
    EXPECT_TRUE (is_valid_digit_string("-0123456789\0"));
    EXPECT_TRUE (is_valid_digit_string("-2147483648"));
    EXPECT_TRUE (is_valid_digit_string("2147483647"));
}
