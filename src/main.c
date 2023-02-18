#include "argparse.h"
#include "baseutil.h"
#include "handler.h"
#include "option.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wasmedge/wasmedge.h>

int main(const int argc, const char **argv) {
  Option *define_opt[] = {create_Option("version", 0, handle_option_version),
                          create_Option("run", -1, handle_option_wasm_arg)};
  Option *remain_arg = create_Option(NULL, -1, handle_option_wasm_arg);
  int define_opt_len = sizeof(define_opt) / sizeof(Option *);
  ParseData *pd = create_ParseData(define_opt, remain_arg, define_opt_len);

  handle_parse(pd, argc, argv);
  show_pd(pd);
  
  handle_option(pd);

  delete_Option(remain_arg);
  delete_Option_array(define_opt, define_opt_len);
  delete_ParseData(pd);
  return 0;
}