
#include "option.h"

#include "baseutil.h"

#include <stdio.h>
#include <stdlib.h>

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

bool show_opt(const Option* opt) {
  if (is_null_ptr(opt, __func__)) return _FAILED;
  printf("opt_name: %s\n", opt->opt_name);
  printf("args_len: %d\n", opt->args_len);
  printf("found: %d\n", opt->found);
  for (int i=0; i<opt->args_len; i++) {
    printf("args[%d]: %s\n", i, opt->args[i]);
  }
  return _SUCCESS;
}