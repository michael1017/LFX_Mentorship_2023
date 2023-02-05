#ifndef ARGPARSE_H
#define ARGPARSE_H

#include "option.h"
#include <stdbool.h>


typedef struct pd ParseData;
struct pd{
  int opt_len;
  Option** opt;
  Option* remain_arg;
};

ParseData* create_ParseData(void);
bool delete_ParseData(ParseData* );

bool show_pd(const ParseData* );

bool _set_option_args(Option* , int* , const int , const char** );

bool get_option_index(int* , const Option** , const int , const char* );

bool handle_parse(ParseData* , Option** , const int , const int , const char** );

#endif /* ARGPARSE_H */