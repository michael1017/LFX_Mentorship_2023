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
void delete_ParseData(ParseData* );

void show_pd(const ParseData* );

bool set_pd_option(ParseData* , const Option** , const int );
bool _set_option_args(Option* , int* , const int , const char** );

bool get_option_index(int* , const Option** , const int , const char* );

bool handle_parse(ParseData* , const int , const char** );

#endif /* ARGPARSE_H */