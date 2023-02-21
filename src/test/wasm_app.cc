extern "C" {
#include "handler.h"

#include "baseutil.h"
}
#include <gtest/gtest.h>

TEST(wasm_app, add) {
  Option *opt = create_Option(NULL, -1, handle_option_wasm_arg);
  char *args[] = {(char *)"wasm_app/wasm/add.wasm", (char *)"1111", (char *)"2222"};
  opt->args_len = sizeof(args) / sizeof(char *);
  opt->args = args;

  EXPECT_EQ(opt->handle_func(opt), _SUCCESS);
  delete_Option(opt);
}

TEST(wasm_app, add3) {
  Option *opt = create_Option(NULL, -1, handle_option_wasm_arg);
  char *args[] = {(char *)"wasm_app/wasm/add3.wasm", (char *)"1111", (char *)"2222",
                  (char *)"3333"};
  opt->args_len = sizeof(args) / sizeof(char *);
  opt->args = args;

  EXPECT_EQ(opt->handle_func(opt), _SUCCESS);
  delete_Option(opt);
}

TEST(wasm_app, circle_f32) {
  Option *opt = create_Option(NULL, -1, handle_option_wasm_arg);
  char *args[] = {(char *)"wasm_app/wasm/circle_area_f32.wasm", (char *)"5"};
  opt->args_len = sizeof(args) / sizeof(char *);
  opt->args = args;

  EXPECT_EQ(opt->handle_func(opt), _SUCCESS);
  delete_Option(opt);
}

TEST(wasm_app, circle_f64) {
  Option *opt = create_Option(NULL, -1, handle_option_wasm_arg);
  char *args[] = {(char *)"wasm_app/wasm/circle_area_f64.wasm", (char *)"5"};
  opt->args_len = sizeof(args) / sizeof(char *);
  opt->args = args;

  EXPECT_EQ(opt->handle_func(opt), _SUCCESS);
  delete_Option(opt);
}

TEST(wasm_app, hello) {
  Option *opt = create_Option(NULL, -1, handle_option_wasm_arg);
  char *args[] = {(char *)"wasm_app/wasm/hello.wasm", (char *)"second", (char *)"state"};
  opt->args_len = sizeof(args) / sizeof(char *);
  opt->args = args;

  EXPECT_EQ(opt->handle_func(opt), _SUCCESS);
  delete_Option(opt);
}

TEST(wasm_app, initfunc) {
  Option *opt = create_Option(NULL, -1, handle_option_wasm_arg);
  char *args[] = {(char *)"wasm_app/wasm/initfunc.wasm"};
  opt->args_len = sizeof(args) / sizeof(char *);
  opt->args = args;

  EXPECT_EQ(opt->handle_func(opt), _SUCCESS);
  delete_Option(opt);
}

TEST(wasm_app, print2023) {
  Option *opt = create_Option(NULL, -1, handle_option_wasm_arg);
  char *args[] = {(char *)"wasm_app/wasm/print2023.wasm"};
  opt->args_len = sizeof(args) / sizeof(char *);
  opt->args = args;

  EXPECT_EQ(opt->handle_func(opt), _SUCCESS);
  delete_Option(opt);
}