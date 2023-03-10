extern "C" {
#include "baseutil.h"

#include "argparse.h"
#include "handler.h"
#include "option.h"
}
#include <gtest/gtest.h>
#include <stdbool.h>

TEST(baseutil, is_null_ptr) {
  const Option *opt_null = NULL;
  const Option *opt = create_Option(NULL, -1, handle_option_null);
  const ParseData *pd_null = NULL;
  const ParseData *pd = create_ParseData(NULL, NULL, 0);
  const char *c_null = NULL;
  const char c = 'c';

  const char func_name[32] = "isNullPtrTest";
  EXPECT_TRUE(is_null_ptr(opt_null, func_name));
  EXPECT_TRUE(is_null_ptr(pd_null, func_name));
  EXPECT_TRUE(is_null_ptr(c_null, func_name));
  EXPECT_FALSE(is_null_ptr(opt, func_name));
  EXPECT_FALSE(is_null_ptr(pd, func_name));
  EXPECT_FALSE(is_null_ptr(&c, func_name));

  delete_Option((Option *)opt);
  delete_ParseData((ParseData *)pd);
}