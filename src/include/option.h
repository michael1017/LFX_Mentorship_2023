/// @file option.h
/// @brief Basic functions for Option operation.
#ifndef OPTION_H
#define OPTION_H

#include <stdbool.h>

/// @brief Option Struct
/// @param opt_name option name.
/// @param args_len args length.
/// @param args char pointer arrary. It contains arguments.
/// @param found initialize to false. Set to true if the option is found.
/// @param handle_func pointer of option handler.
typedef struct opt {
  char *opt_name;
  int args_len;
  char **args;
  bool found;
  bool (*handle_func)(const struct opt *);
} Option;

/// @brief Create a ParseData pointer, need to be deleted before exit execution.
/// @param opt_name is the name of option.
/// @param args_len is the expected length of arguments. -1 means the option will accept
/// all arguments after it.
/// @param func_ptr pointer of option handler (option handler should be a function).
/// @return return Option pointer.
Option *create_Option(char *opt_name, int args_len, bool (*handle_func)(const Option *));

/// @brief Delete a Option pointer from create_Option().
/// @param opt is a Option pointer.
void delete_Option(Option *opt);

/// @brief Delete a Option pointer array.
/// @param opt is a Option pointer array.
/// @param opt_len is the length of Option pointer array.
void delete_Option_array(Option **opt, int opt_len);

/// @brief Print Option
/// @param opt is a Option pointer
void show_opt(const Option *opt);

#endif /* OPTION_H */