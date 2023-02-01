#ifndef ARGPARSE_H
#define ARGPARSE_H

#define _BOLD_RED "\033[1m\033[31m"
#define _RESET_COLOR "\033[0m"
#define _SUCCESS true
#define _FAILED false
#define _NOT_FOUND -1

typedef struct pd ParseData;
typedef struct opt Option;

struct pd{
  int opt_len;
  Option** opt;
  Option* remain_arg;
};

struct opt{
  char* opt_name;
  int args_len;
  char** args;
  bool found;
};

bool is_null_ptr(const void* , const char* );

Option* create_Option(char* , int );
void delete_Option(Option* );

ParseData* create_ParseData(void);
void delete_ParseData(ParseData* );

void show_pd(const ParseData* );
void show_opt(const Option* );

bool set_pd_option(ParseData* , const Option** , const int );
bool _set_option_args(Option* , int* , const int , const char** );

bool get_option_index(int* , const Option** , const int , const char* );

bool handle_parse(ParseData* , const int , const char** );
#endif /* ARGPARSE_H */