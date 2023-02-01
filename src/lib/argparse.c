#include <wasmedge/wasmedge.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "argparse.h"

bool is_null_ptr(const void* ptr, const char* func_name) {
  if (ptr != NULL) return false;
  fprintf(stderr, _BOLD_RED "ERROR =>" _RESET_COLOR \
    " function '%s' get null pointer\n", func_name);
  return true;
}

Option* create_Option(char* opt_name, int args_len) {
  Option* opt = malloc(sizeof(Option));
  opt->opt_name = opt_name;
  opt->args_len = args_len;
  opt->args = NULL;
  opt->found = false;
  return opt;
}

void delete_Option(Option* opt) {
  free(opt);
  return;
}

ParseData* create_ParseData(void) {
  ParseData* pd = malloc(sizeof(ParseData));
  pd->opt_len = 0;
  pd->opt = NULL;
  pd->remain_arg = create_Option(NULL, -1);
  return pd;
}

void delete_ParseData(ParseData* pd) {
  delete_Option(pd->remain_arg);
  free(pd);
  return;
}

void show_pd(const ParseData* pd) {
  for (int i=0; i<pd->opt_len; i++) {
    printf("---option %d---\n", i);
    show_opt(pd->opt[i]);
    printf("\n");
  }

  printf("---remain_arg---\n");
  show_opt(pd->remain_arg);
  return;
}

void show_opt(const Option* opt) {
  printf("opt_name: %s\n", opt->opt_name);
  printf("args_len: %d\n", opt->args_len);
  printf("found: %d\n", opt->found);
  for (int i=0; i<opt->args_len; i++) {
    printf("args[%d]: %s\n", i, opt->args[i]);
  }
  return;
}

bool set_pd_option(ParseData* pd, const Option** opt, const int opt_len) {
  if (is_null_ptr(pd, __func__)) return _FAILED;

  pd->opt_len = opt_len;
  pd->opt = (Option** )opt;

  return _SUCCESS;
}

bool _set_option_args(Option* opt, int* arg_idx, const int argc, const char** argv) {
  if (is_null_ptr(opt, __func__)) return _FAILED;
  if (is_null_ptr(arg_idx, __func__)) return _FAILED;
  if (*arg_idx >= argc) return _FAILED; // illegal arg_idx

  int remain_arg_len = argc - *arg_idx;
  if (opt->args_len < 0) opt->args_len = remain_arg_len; // get all args
  if (opt->args_len > remain_arg_len) return _FAILED; // too few args

  // set args, found
  opt->found = true;
  opt->args = (char** )argv + *arg_idx; // args head

  // update arg_idx
  arg_idx[0] += opt->args_len;

  return _SUCCESS;

}

bool get_option_index(int* index, const Option** opt, const int opt_len, const char* target) {
  if (is_null_ptr(index, __func__)) return _FAILED;

  // Found
  for (int i=0; i<opt_len; i++) {
    if (strcmp(opt[i]->opt_name, target) == 0) {
      *index = i;
      return _SUCCESS;
    }
  }
  
  // Not Found
  *index = _NOT_FOUND;
  return _SUCCESS;
}

bool handle_parse(ParseData* pd, const int argc, const char** argv) {
  if (is_null_ptr(pd, __func__)) return _FAILED;
  int arg_idx = 1;

  int opt_idx;
  bool state;
  while (arg_idx < argc) {
    // get option index
    state = get_option_index(&opt_idx, (const Option ** )pd->opt, pd->opt_len, argv[arg_idx]);
    if (state == _FAILED) return _FAILED;

    // pass args
    if (opt_idx == _NOT_FOUND) { // pass args for remain_arg
      state = _set_option_args(pd->remain_arg, &arg_idx, argc, argv);
    } 
    else { // pass args for options
      arg_idx++;
      state = _set_option_args(pd->opt[opt_idx], &arg_idx, argc, argv);
    }
    if (state == _FAILED) return _FAILED;
  }
  return _SUCCESS;
}