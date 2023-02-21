/// @file baseutil.h
/// @brief Basic functions for the whole project.
#ifndef BASEUTIL_H
#define BASEUTIL_H

#include <stdbool.h>

#define _BOLD_RED "\033[1m\033[31m"
#define _RESET_COLOR "\033[0m"
#define _ERROR_SIG _BOLD_RED "ERROR => " _RESET_COLOR
#define _SUCCESS true
#define _FAILED false
#define _NOT_FOUND -1

/// @brief Check if the pointer is a null pointer.
/// @param ptr the pointer to check is null or not.
/// @param func_name Use __func__ to identify which function found a null pointer.
/// @return If is null pointer, return true. If not, return false.
bool is_null_ptr(const void *ptr, const char *func_name);

#endif /* BASEUTIL_H */