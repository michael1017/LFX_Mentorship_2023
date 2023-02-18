extern "C" {
#include "handler.h"

#include "baseutil.h"
}
#include <gtest/gtest.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wasmedge/wasmedge.h>

TEST(handler, handle_option_version) { EXPECT_EQ(handle_option_version(NULL), _SUCCESS); }

TEST(handler, handle_option_wasm_arg) {
  // The Test may fail due to wasm file non-exist
  Option *opt = create_Option(NULL, -1, handle_option_wasm_arg);
  opt->found = true;
  { // add.wasm
    char *args[] = {(char *)"wasm_app/wasm/add.wasm", (char *)"1111", (char *)"2222"};
    opt->args_len = sizeof(args) / sizeof(char *);
    opt->args = args;

    EXPECT_EQ(opt->handle_func(opt), _SUCCESS);
  }
  { // add3.wasm
    char *args[] = {(char *)"wasm_app/wasm/add3.wasm", (char *)"1111", (char *)"2222",
                    (char *)"3333"};
    opt->args_len = sizeof(args) / sizeof(char *);
    opt->args = args;

    EXPECT_EQ(opt->handle_func(opt), _SUCCESS);
  }
  { // addall.wasm
    char *args[] = {(char *)"wasm_app/wasm/addall.wasm",
                    (char *)"1111",
                    (char *)"2222",
                    (char *)"3333",
                    (char *)"1111",
                    (char *)"1111"};
    opt->args_len = sizeof(args) / sizeof(char *);
    opt->args = args;

    EXPECT_EQ(opt->handle_func(opt), _SUCCESS);
  }
  { // circle_area_f32.wasm
    char *args[] = {(char *)"wasm_app/wasm/circle_area_f32.wasm", (char *)"5"};
    opt->args_len = sizeof(args) / sizeof(char *);
    opt->args = args;

    EXPECT_EQ(opt->handle_func(opt), _SUCCESS);
  }
  { // circle_area_f64.wasm
    char *args[] = {(char *)"wasm_app/wasm/circle_area_f64.wasm", (char *)"5"};
    opt->args_len = sizeof(args) / sizeof(char *);
    opt->args = args;

    EXPECT_EQ(opt->handle_func(opt), _SUCCESS);
  }
  { // hello.wasm
    char *args[] = {(char *)"wasm_app/wasm/hello.wasm", (char *)"second",
                    (char *)"state"};
    opt->args_len = sizeof(args) / sizeof(char *);
    opt->args = args;

    EXPECT_EQ(opt->handle_func(opt), _SUCCESS);
  }
  { // print2023.wasm
    char *args[] = {(char *)"wasm_app/wasm/print2023.wasm", (char *)"second",
                    (char *)"state"};
    opt->args_len = sizeof(args) / sizeof(char *);
    opt->args = args;

    EXPECT_EQ(opt->handle_func(opt), _SUCCESS);
  }

  // Failed Cases
  { // Wrong Path
    char *args[] = {(char *)"wasm_app/wasm/hello.wa", (char *)"second", (char *)"state"};
    opt->args_len = sizeof(args) / sizeof(char *);
    opt->args = args;

    EXPECT_EQ(opt->handle_func(opt), _FAILED);
  }
  { // Too Few Args
    char *args[] = {(char *)"wasm_app/wasm/add.wasm", (char *)"1111"};
    opt->args_len = sizeof(args) / sizeof(char *);
    opt->args = args;

    EXPECT_EQ(opt->handle_func(opt), _FAILED);
  }
  { // Too Much Args
    char *args[] = {(char *)"wasm_app/wasm/add.wasm", (char *)"1111", (char *)"1111",
                    (char *)"1111"};
    opt->args_len = sizeof(args) / sizeof(char *);
    opt->args = args;

    EXPECT_EQ(opt->handle_func(opt), _FAILED);
  }
  delete_Option(opt);
}

TEST(handler, handle_option) {
  // The Test may fail due to wasm file non-exist
  {
    Option *define_opt[] = {create_Option((char *)"version", 0, handle_option_version),
                            create_Option((char *)"run", -1, handle_option_wasm_arg)};
    Option *remain_arg = create_Option(NULL, -1, handle_option_wasm_arg);
    int define_opt_len = sizeof(define_opt) / sizeof(Option *);
    ParseData *pd = create_ParseData(define_opt, remain_arg, define_opt_len);

    const char *argv[] = {"./execution", "version", "run", "wasm_app/wasm/add.wasm",
                          "-22222",      "11111"};
    const int argc = sizeof(argv) / sizeof(char *);

    handle_parse(pd, argc, argv);
    EXPECT_EQ(handle_option(pd), _SUCCESS);

    delete_Option(remain_arg);
    delete_Option_array(define_opt, define_opt_len);
    delete_ParseData(pd);
  }
  {
    Option *define_opt[] = {create_Option((char *)"version", 0, handle_option_version),
                            create_Option((char *)"run", -1, handle_option_wasm_arg)};
    Option *remain_arg = create_Option(NULL, -1, handle_option_wasm_arg);
    int define_opt_len = sizeof(define_opt) / sizeof(Option *);
    ParseData *pd = create_ParseData(define_opt, remain_arg, define_opt_len);

    const char *argv[] = {"./execution", "version", "wasm_app/wasm/add.wasm", "-22222",
                          "11111"};
    const int argc = sizeof(argv) / sizeof(char *);

    handle_parse(pd, argc, argv);
    EXPECT_EQ(handle_option(pd), _SUCCESS);

    delete_Option(remain_arg);
    delete_Option_array(define_opt, define_opt_len);
    delete_ParseData(pd);
  }
  {
    Option *define_opt[] = {create_Option((char *)"version", 0, handle_option_version),
                            create_Option((char *)"run", -1, handle_option_wasm_arg)};
    Option *remain_arg = create_Option(NULL, -1, handle_option_wasm_arg);
    int define_opt_len = sizeof(define_opt) / sizeof(Option *);
    ParseData *pd = create_ParseData(define_opt, remain_arg, define_opt_len);

    const char *argv[] = {"./execution", "version", "wasm_app/wasm/add.wasm",
                          "-22222",      "11111",   "123"};
    const int argc = sizeof(argv) / sizeof(char *);

    handle_parse(pd, argc, argv);
    EXPECT_EQ(handle_option(pd), _FAILED);

    delete_Option(remain_arg);
    delete_Option_array(define_opt, define_opt_len);
    delete_ParseData(pd);
  }
}