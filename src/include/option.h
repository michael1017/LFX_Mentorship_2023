#ifndef OPTION_H
#define OPTION_H

#include <stdbool.h>

typedef struct opt Option;
struct opt{
  char* opt_name;
  int args_len;
  char** args;
  bool found;
};

Option* create_Option(char* , int );
void delete_Option(Option* );
void delete_Option_array(Option** , int );

bool show_opt(const Option* );

#endif /* OPTION_H */