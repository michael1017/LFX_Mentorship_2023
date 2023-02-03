#ifndef BASEUTIL_H
#define BASEUTIL_H

#include <stdbool.h>

#define _BOLD_RED "\033[1m\033[31m"
#define _RESET_COLOR "\033[0m"
#define _ERROR_SIG _BOLD_RED "ERROR => " _RESET_COLOR
#define _SUCCESS true
#define _FAILED false
#define _NOT_FOUND -1

bool is_null_ptr(const void* , const char* );
bool is_file_exist(const char* );
bool is_valid_digit_string(const char* );

#endif /* BASEUTIL_H */