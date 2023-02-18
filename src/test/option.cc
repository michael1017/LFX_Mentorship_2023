extern "C" {
#include "option.h"

#include "baseutil.h"
#include "handler.h"
}
#include <gtest/gtest.h>
#include <stdbool.h>

TEST(option, create_Option) {
  char opt2_name[] = "version";
  char opt3_name[] = "run";
  Option *opt1 = create_Option(NULL, 3, handle_option_null);
  Option *opt2 = create_Option(opt2_name, 0, handle_option_null);
  Option *opt3 = create_Option(opt3_name, -1, handle_option_null);

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

  delete_Option(opt1);
  delete_Option(opt2);
  delete_Option(opt3);
}