/// @file argparse.h
/// @brief Provides stuff for argument parsing and return raw parsing result.
#ifndef ARGPARSE_H
#define ARGPARSE_H

#include "option.h"
#include <stdbool.h>

/// @brief ParseData Struct
/// @param opt is a Option pointer array.
/// @param remain_arg is a Option pointer. It contains arguments not belong to any option.
/// @param opt_len is the length of opt
typedef struct pd {
  Option **opt;
  Option *remain_arg;
  int opt_len;
} ParseData;

/// @brief Create a ParseData pointer, need to be deleted before exit execution.
/// @param opt is a Option pointer array.
/// @param remain_arg is a Option pointer. It contains arguments not belong to any option.
/// @param opt_len is the length of opt
/// @return Return ParseData pointer.
ParseData *create_ParseData(Option **opt, Option *remain_arg, int opt_len);

/// @brief Delete a ParseData pointer from create_ParseData().
/// @param pd ParseData pointer
void delete_ParseData(ParseData *pd);

/// @brief Print ParseData
/// @param pd is a ParseData pointer
void show_pd(const ParseData *pd);

bool _set_option_args(Option *, int *, const int, const char **);

bool _get_option_index(int *, const Option **, const int, const char *);

/// @brief Parse argv and return result to ParseData pointer
/// @param pd is a ParseData pointer. It is used to get parsing result.
/// @param argc is length of argv.
/// @param argv constains arguments pass from CLI.
/// @return Return true if success. Return false if not.
bool handle_parse(ParseData *pd, const int argc, const char **argv);

#endif /* ARGPARSE_H */