
#include "option.h"

#include "baseutil.h"

#include <stdio.h>
#include <stdlib.h>

Option *create_Option(char *opt_name, int args_len, bool (*handle_func)(const Option *)) {
  Option *opt = malloc(sizeof(Option));
  opt->opt_name = opt_name;
  opt->args_len = args_len;
  opt->args = NULL;
  opt->found = false;
  opt->handle_func = handle_func;
  return opt;
}

void delete_Option(Option *opt) {
  if (opt == NULL)
    return;
  free(opt);
  return;
}

void delete_Option_array(Option **opt, int opt_len) {
  if (opt == NULL || opt_len <= 0)
    return;
  for (int i = 0; i < opt_len; i++) {
    delete_Option(opt[i]);
  }
  return;
}

void show_opt(const Option *opt) {
  if (opt == NULL)
    return;
  printf("opt_name: %s\n", opt->opt_name == NULL ? "NULL" : opt->opt_name);
  printf("args_len: %d\n", opt->args_len);
  printf("found: %d\n", opt->found);
  for (int i = 0; i < opt->args_len; i++) {
    printf("args[%d]: %s\n", i, opt->args[i]);
  }
  return;
}