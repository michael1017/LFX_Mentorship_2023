#include "argparse.h"

#include "baseutil.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ParseData *create_ParseData(void) {
  ParseData *pd = malloc(sizeof(ParseData));
  pd->opt_len = 0;
  pd->opt = NULL;
  pd->remain_arg = create_Option(NULL, -1);
  return pd;
}

bool delete_ParseData(ParseData *pd) {
  if (is_null_ptr(pd, __func__))
    return _FAILED;
  delete_Option(pd->remain_arg);
  free(pd);
  return _SUCCESS;
}

bool show_pd(const ParseData *pd) {
  if (is_null_ptr(pd, __func__))
    return _FAILED;

  printf("Total Option Length: %d\n", pd->opt_len);
  for (int i = 0; i < pd->opt_len; i++) {
    printf("---option %d---\n", i);
    show_opt(pd->opt[i]);
    printf("\n");
  }

  printf("---remain_arg---\n");
  show_opt(pd->remain_arg);
  return _SUCCESS;
}

bool _set_option_args(Option *opt, int *arg_idx, const int argc, const char **argv) {
  if (is_null_ptr(opt, __func__))
    return _FAILED;
  if (is_null_ptr(arg_idx, __func__))
    return _FAILED;
  if (*arg_idx >= argc && opt->args_len > 0) { // illegal arg_idx
    fprintf(stderr, _ERROR_SIG "%s: illegal arg_idx, arg_idx >= argc\n", __func__);
    return _FAILED;
  }

  int remain_arg_len = argc - *arg_idx;
  if (opt->args_len < 0)
    opt->args_len = remain_arg_len;     // get all args
  if (opt->args_len > remain_arg_len) { // too few args
    fprintf(stderr, _ERROR_SIG "%s: Too few args for the option: %s\n", __func__,
            opt->opt_name);
    return _FAILED;
  }

  // set args, found
  opt->found = true;
  opt->args = (char **)argv + *arg_idx; // args head

  // update arg_idx
  arg_idx[0] += opt->args_len;

  return _SUCCESS;
}

bool get_option_index(int *index, const Option **opt, const int opt_len,
                      const char *target) {
  if (is_null_ptr(index, __func__))
    return _FAILED;
  if (opt == NULL || opt_len <= 0) {
    // Not Found
    *index = _NOT_FOUND;
    return _SUCCESS;
  }

  // Found
  for (int i = 0; i < opt_len; i++) {
    if (strcmp(opt[i]->opt_name, target) == 0) {
      *index = i;
      return _SUCCESS;
    }
  }

  // Not Found
  *index = _NOT_FOUND;
  return _SUCCESS;
}

bool handle_parse(ParseData *pd, Option **opt, const int opt_len, const int argc,
                  const char **argv) {
  if (is_null_ptr(pd, __func__))
    return _FAILED;

  int arg_idx = 1;
  int opt_idx;
  bool state;

  pd->opt_len = opt_len;
  pd->opt = opt;

  while (arg_idx < argc) {
    // get option index
    state =
        get_option_index(&opt_idx, (const Option **)pd->opt, pd->opt_len, argv[arg_idx]);
    if (state == _FAILED) {
      fprintf(stderr, _ERROR_SIG "%s: 'get_option_index' failed\n", __func__);
      return _FAILED;
    }
    // pass args
    if (opt_idx == _NOT_FOUND) { // pass args for remain_arg
      state = _set_option_args(pd->remain_arg, &arg_idx, argc, argv);
    } else { // pass args for options
      arg_idx++;
      state = _set_option_args(pd->opt[opt_idx], &arg_idx, argc, argv);
    }
    if (state == _FAILED) {
      fprintf(stderr, _ERROR_SIG "%s: '_set_option_args' failed\n", __func__);
      return _FAILED;
    }
  }
  return _SUCCESS;
}