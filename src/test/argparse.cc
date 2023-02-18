extern "C" {
#include "argparse.h"

#include "baseutil.h"
#include "handler.h"
#include "option.h"
}
#include <gtest/gtest.h>
#include <stdbool.h>
#include <stdio.h>


TEST(argparse, create_ParseData) {
  ParseData *pd = NULL;
  Option *define_opt[] = {create_Option((char *)"d", 3, handle_option_null),
                          create_Option((char *)"a", 0, handle_option_null),
                          create_Option((char *)"b", 2, handle_option_null),
                          create_Option((char *)"c", -1, handle_option_null)

  };
  Option *remain_arg = create_Option((char*)NULL, -1, handle_option_null);
  const int define_opt_len = sizeof(define_opt) / sizeof(Option *);
  pd = create_ParseData(define_opt, remain_arg, define_opt_len);

  EXPECT_NE(pd, nullptr);
  delete_Option(remain_arg);
  delete_Option_array(define_opt, define_opt_len);
  delete_ParseData(pd);
}

TEST(argparse, _set_option_args) {
  const char *fake_argv1[] = {"./x", "a", "b", "b1", "b2", "c", "c1"};
  const char *fake_argv2[] = {"./x", "b", "b1"};

  const int fake_argc1 = sizeof(fake_argv1) / sizeof(char *);
  const int fake_argc2 = sizeof(fake_argv2) / sizeof(char *);

  int start_point;
  int arg_idx1, arg_idx2;

  Option *define_opt[] = {create_Option((char *)"a", 0, handle_option_null),
                          create_Option((char *)"b", 2, handle_option_null),
                          create_Option((char *)"c", -1, handle_option_null)};
  int define_opt_len = sizeof(define_opt) / sizeof(Option *);

  // NULL POINTER TEST
  EXPECT_EQ(_set_option_args(NULL, NULL, fake_argc1, fake_argv1), _FAILED);
  EXPECT_EQ(_set_option_args(define_opt[0], NULL, fake_argc1, fake_argv1), _FAILED);
  EXPECT_EQ(_set_option_args(NULL, &arg_idx1, fake_argc1, fake_argv1), _FAILED);

  // JUMP TEST PRE-TEST
  EXPECT_FALSE(define_opt[0]->found);
  EXPECT_FALSE(define_opt[1]->found);
  EXPECT_FALSE(define_opt[2]->found);
  // JUMP TEST 1-1
  start_point = 2;
  arg_idx1 = start_point;
  EXPECT_EQ(_set_option_args(define_opt[0], &arg_idx1, fake_argc1, fake_argv1), _SUCCESS);
  EXPECT_EQ(arg_idx1, start_point + define_opt[0]->args_len);
  EXPECT_TRUE(define_opt[0]->found); // set -> true
  EXPECT_FALSE(define_opt[1]->found);
  EXPECT_FALSE(define_opt[2]->found);
  // JUMP TEST 1-2
  start_point = 3;
  arg_idx1 = start_point;
  EXPECT_EQ(_set_option_args(define_opt[1], &arg_idx1, fake_argc1, fake_argv1), _SUCCESS);
  EXPECT_EQ(arg_idx1, start_point + define_opt[1]->args_len);
  EXPECT_TRUE(define_opt[0]->found);
  EXPECT_TRUE(define_opt[1]->found); // set -> true
  EXPECT_FALSE(define_opt[2]->found);
  // JUMP TEST 1-3
  start_point = 6;
  arg_idx1 = start_point;
  EXPECT_EQ(_set_option_args(define_opt[2], &arg_idx1, fake_argc1, fake_argv1), _SUCCESS);
  EXPECT_EQ(arg_idx1, start_point + define_opt[2]->args_len);
  EXPECT_EQ(arg_idx1, fake_argc1);
  EXPECT_TRUE(define_opt[0]->found);
  EXPECT_TRUE(define_opt[1]->found);
  EXPECT_TRUE(define_opt[2]->found); // set -> true

  // TOO FEW ARG TEST
  start_point = 2;
  arg_idx2 = start_point;
  EXPECT_EQ(_set_option_args(define_opt[1], &arg_idx2, fake_argc2, fake_argv2), _FAILED);

  // ARG_IDX RANGE TEST
  arg_idx2 = fake_argc2;
  EXPECT_EQ(_set_option_args(define_opt[2], &arg_idx2, fake_argc2, fake_argv2), _FAILED);

  delete_Option_array(define_opt, define_opt_len);
}

TEST(argparse, _get_option_index) {
  int index;

  Option *define_opt[] = {create_Option((char *)"a", 0, handle_option_null),
                          create_Option((char *)"b", 2, handle_option_null),
                          create_Option((char *)"c", -1, handle_option_null)};
  int define_opt_len = sizeof(define_opt) / sizeof(Option *);
  // NULL INDEX TEST
  EXPECT_EQ(_get_option_index(NULL, (const Option **)define_opt, define_opt_len,
                              (const char *)"a"),
            _FAILED);

  // NULL OPT TEST
  EXPECT_EQ(_get_option_index(&index, NULL, define_opt_len, (const char *)"a"), _SUCCESS);
  EXPECT_EQ(index, _NOT_FOUND);

  // TEST 1-1
  EXPECT_EQ(_get_option_index(&index, (const Option **)define_opt, define_opt_len,
                              (const char *)"a"),
            _SUCCESS);
  EXPECT_EQ(index, 0);
  // TEST 1-2
  EXPECT_EQ(_get_option_index(&index, (const Option **)define_opt, define_opt_len,
                              (const char *)"b"),
            _SUCCESS);
  EXPECT_EQ(index, 1);
  // TEST 1-3
  EXPECT_EQ(_get_option_index(&index, (const Option **)define_opt, define_opt_len,
                              (const char *)"c"),
            _SUCCESS);
  EXPECT_EQ(index, 2);
  // TEST 1-4
  EXPECT_EQ(_get_option_index(&index, (const Option **)define_opt, define_opt_len,
                              (const char *)"d"),
            _SUCCESS);
  EXPECT_EQ(index, _NOT_FOUND);

  delete_Option_array(define_opt, define_opt_len);
}

TEST(argparse, handle_parse) {
  { // TEST 1

    Option *define_opt[] = {create_Option((char *)"d", 3, handle_option_null),
                            create_Option((char *)"a", 0, handle_option_null),
                            create_Option((char *)"b", 2, handle_option_null),
                            create_Option((char *)"c", -1, handle_option_null)

    };
    Option *remain_arg = create_Option(NULL, -1, handle_option_null);
    const int define_opt_len = sizeof(define_opt) / sizeof(Option *);
    ParseData *pd = create_ParseData(define_opt, remain_arg, define_opt_len);

    const char *fake_argv[] = {"./x", "a", "b", "b1", "b2", "c", "c1"};
    const int fake_argc = sizeof(fake_argv) / sizeof(char *);
    printf("hello1\n");
    handle_parse(pd, fake_argc, fake_argv);
    printf("hello2\n");
    EXPECT_EQ(pd->opt_len, define_opt_len);
    EXPECT_NE(pd->opt, nullptr);

    EXPECT_EQ(pd->opt[0]->args, nullptr);
    EXPECT_FALSE(pd->opt[0]->found);

    EXPECT_EQ(pd->opt[1]->args, fake_argv + 2);
    EXPECT_TRUE(pd->opt[1]->found);

    EXPECT_EQ(pd->opt[2]->args, fake_argv + 3);
    EXPECT_TRUE(pd->opt[2]->found);

    EXPECT_EQ(pd->opt[3]->args, fake_argv + 6);
    EXPECT_TRUE(pd->opt[3]->found);

    delete_Option(remain_arg);
    delete_Option_array(define_opt, define_opt_len);
    delete_ParseData(pd);
  }
  printf("hello3\n");
  { // TEST 2

    Option *define_opt[] = {create_Option((char *)"d", 3, handle_option_null),
                            create_Option((char *)"a", 0, handle_option_null),
                            create_Option((char *)"b", 2, handle_option_null),
                            create_Option((char *)"c", -1, handle_option_null)

    };
    Option *remain_arg = create_Option(NULL, -1, handle_option_null);
    const int define_opt_len = sizeof(define_opt) / sizeof(Option *);
    ParseData *pd = create_ParseData(define_opt, remain_arg, define_opt_len);

    const char *fake_argv[] = {"./x", "z", "z1", "z2"};
    const int fake_argc = sizeof(fake_argv) / sizeof(char *);

    handle_parse(pd, fake_argc, fake_argv);

    EXPECT_EQ(pd->opt[0]->args, nullptr);
    EXPECT_EQ(pd->opt[1]->args, nullptr);
    EXPECT_EQ(pd->opt[2]->args, nullptr);
    EXPECT_EQ(pd->opt[3]->args, nullptr);
    EXPECT_FALSE(pd->opt[0]->found);
    EXPECT_FALSE(pd->opt[1]->found);
    EXPECT_FALSE(pd->opt[2]->found);
    EXPECT_FALSE(pd->opt[3]->found);

    EXPECT_EQ(pd->remain_arg->args, fake_argv + 1);
    EXPECT_TRUE(pd->remain_arg->found);

    delete_Option(remain_arg);
    delete_Option_array(define_opt, define_opt_len);
    delete_ParseData(pd);
  }
}