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

TEST(handler, handle_option_version) { EXPECT_EQ(handle_option_version(), _SUCCESS); }

TEST(handler, handle_option_wasm_arg) {
  // The Test may fail due to wasm file non-exist
  Option *opt = create_Option(NULL, -1);
  opt->found = true;
  {
    char *args[] = {(char *)"wasm_app/wasm/add.wasm", (char *)"1111", (char *)"2222"};
    opt->args_len = sizeof(args) / sizeof(char *);
    opt->args = args;

    EXPECT_EQ(handle_option_wasm_arg(opt), _SUCCESS);
  }
  {
    char *args[] = {(char *)"wasm_app/wasm/add3.wasm", (char *)"1111", (char *)"2222",
                    (char *)"3333"};
    opt->args_len = sizeof(args) / sizeof(char *);
    opt->args = args;

    EXPECT_EQ(handle_option_wasm_arg(opt), _SUCCESS);
  }
  {
    char *args[] = {(char *)"wasm_app/wasm/circle_area_f32.wasm", (char *)"5"};
    opt->args_len = sizeof(args) / sizeof(char *);
    opt->args = args;

    EXPECT_EQ(handle_option_wasm_arg(opt), _SUCCESS);
  }
  {
    char *args[] = {(char *)"wasm_app/wasm/circle_area_f64.wasm", (char *)"5"};
    opt->args_len = sizeof(args) / sizeof(char *);
    opt->args = args;

    EXPECT_EQ(handle_option_wasm_arg(opt), _SUCCESS);
  }
  {
    char *args[] = {(char *)"wasm_app/wasm/hello.wasm", (char *)"second",
                    (char *)"state"};
    opt->args_len = sizeof(args) / sizeof(char *);
    opt->args = args;

    EXPECT_EQ(handle_option_wasm_arg(opt), _SUCCESS);
  }
  {
    char *args[] = {(char *)"wasm_app/wasm/print2023.wasm", (char *)"second",
                    (char *)"state"};
    opt->args_len = sizeof(args) / sizeof(char *);
    opt->args = args;

    EXPECT_EQ(handle_option_wasm_arg(opt), _SUCCESS);
  }
  {
    char *args[] = {(char *)"wasm_app/wasm/hello.wa", (char *)"second", (char *)"state"};
    opt->args_len = sizeof(args) / sizeof(char *);
    opt->args = args;

    EXPECT_EQ(handle_option_wasm_arg(opt), _FAILED);
  }
  {
    char *args[] = {(char *)"wasm_app/wasm/add.wasm", (char *)"1111"};
    opt->args_len = sizeof(args) / sizeof(char *);
    opt->args = args;

    EXPECT_EQ(handle_option_wasm_arg(opt), _FAILED);
  }
  {
    char *args[] = {(char *)"wasm_app/wasm/add.wasm", (char *)"1111", (char *)"1111",
                    (char *)"1111"};
    opt->args_len = sizeof(args) / sizeof(char *);
    opt->args = args;

    EXPECT_EQ(handle_option_wasm_arg(opt), _FAILED);
  }
  delete_Option(opt);
}

TEST(handler, handle_option) {
  // The Test may fail due to wasm file non-exist
  {
    ParseData *pd = create_ParseData();
    Option *define_opt[] = {create_Option((char *)"version", 0),
                            create_Option((char *)"run", -1)};
    int define_opt_len = sizeof(define_opt) / sizeof(Option *);
    const char *argv[] = {"./execution", "version", "run", "wasm_app/wasm/add.wasm",
                          "-22222",      "11111"};
    const int argc = sizeof(argv) / sizeof(char *);

    handle_parse(pd, define_opt, define_opt_len, argc, argv);
    EXPECT_EQ(handle_option(pd), _SUCCESS);

    delete_Option_array(define_opt, define_opt_len);
    delete_ParseData(pd);
  }
  {
    ParseData *pd = create_ParseData();
    Option *define_opt[] = {create_Option((char *)"version", 0),
                            create_Option((char *)"run", -1)};
    int define_opt_len = sizeof(define_opt) / sizeof(Option *);
    const char *argv[] = {"./execution", "version", "wasm_app/wasm/add.wasm", "-22222",
                          "11111"};
    const int argc = sizeof(argv) / sizeof(char *);

    handle_parse(pd, define_opt, define_opt_len, argc, argv);
    EXPECT_EQ(handle_option(pd), _SUCCESS);

    delete_Option_array(define_opt, define_opt_len);
    delete_ParseData(pd);
  }
  {
    ParseData *pd = create_ParseData();
    Option *define_opt[] = {create_Option((char *)"version", 0),
                            create_Option((char *)"run", -1)};
    int define_opt_len = sizeof(define_opt) / sizeof(Option *);
    const char *argv[] = {"./execution", "version", "wasm_app/wasm/add.wasm",
                          "-22222",      "11111",   "123"};
    const int argc = sizeof(argv) / sizeof(char *);

    handle_parse(pd, define_opt, define_opt_len, argc, argv);
    EXPECT_EQ(handle_option(pd), _FAILED);

    delete_Option_array(define_opt, define_opt_len);
    delete_ParseData(pd);
  }
}