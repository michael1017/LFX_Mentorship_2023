#include "argparse.h"
#include "baseutil.h"
#include "handler.h"
#include "option.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wasmedge/wasmedge.h>

int main(const int argc, const char **argv) {
  ParseData *pd = create_ParseData();
  Option *define_opt[] = {create_Option("version", 0), create_Option("run", -1)};
  int define_opt_len = sizeof(define_opt) / sizeof(Option *);

  handle_parse(pd, (Option **)define_opt, define_opt_len, argc, argv);
  // show_pd(pd);

  handle_option(pd);

  delete_Option_array(define_opt, define_opt_len);
  delete_ParseData(pd);
  return 0;
}