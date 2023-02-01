#include "option.h"
#include "argparse.h"

#include <wasmedge/wasmedge.h>
#include <stdio.h>
#include <stdlib.h>

int main(const int argc, const char** argv) {
  ParseData* pd = create_ParseData();
  Option* define_opt[] = {create_Option("version", 0), create_Option("run", -1)};

  set_pd_option(pd, (const Option ** )define_opt, 2);
  handle_parse(pd, argc, argv);
  show_pd(pd);

  delete_Option(define_opt[0]);
  delete_Option(define_opt[1]);
  delete_ParseData(pd);
  return 0;
}
