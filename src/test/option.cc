extern "C" {
    #include "option.h"

    #include "baseutil.h"
}
#include <stdbool.h>
#include <gtest/gtest.h>

TEST(option, create_Option) {
    char opt2_name[] = "version";
    char opt3_name[] = "run";
    Option* opt1 = create_Option(NULL, 3);
    Option* opt2 = create_Option(opt2_name, 0);
    Option* opt3 = create_Option(opt3_name, -1);
    Option* opt4 = NULL;
    opt4 = create_Option(NULL, 3);

    EXPECT_EQ(opt1->opt_name, nullptr);
    EXPECT_EQ(opt1->args_len, 3);
    EXPECT_EQ(opt1->args, nullptr);
    EXPECT_EQ(opt1->found, false);

    EXPECT_STREQ(opt2->opt_name, opt2_name);
    EXPECT_EQ(opt2->args_len, 0);
    EXPECT_EQ(opt2->args, nullptr);
    EXPECT_EQ(opt2->found, false);

    EXPECT_STREQ(opt3->opt_name, opt3_name);
    EXPECT_EQ(opt3->args_len, -1);
    EXPECT_EQ(opt3->args, nullptr);
    EXPECT_EQ(opt3->found, false);

    EXPECT_NE(opt4, nullptr);

    delete_Option(opt1);
    delete_Option(opt2);
    delete_Option(opt3);
    delete_Option(opt4);
}

TEST(option, delete_Option) {
    Option* opt = create_Option(NULL, -1);
    EXPECT_EQ(delete_Option(NULL), _FAILED);
    EXPECT_EQ(delete_Option(opt), _SUCCESS);
}

TEST(option, delete_Option_array) {
    Option* define_opt[] = {create_Option((char*)"version", 0), create_Option((char*)"run", -1)};

    EXPECT_EQ(delete_Option_array(NULL, -1), _FAILED);
    EXPECT_EQ(delete_Option_array(NULL, 10), _FAILED);
    EXPECT_EQ(delete_Option_array(define_opt, -1), _FAILED);
    EXPECT_EQ(delete_Option_array(define_opt, 2), _SUCCESS);
}

TEST(option, show_opt) {
    Option* opt = create_Option(NULL, -1);
    EXPECT_EQ(show_opt(NULL), _FAILED);
    EXPECT_EQ(show_opt(opt), _SUCCESS);
}