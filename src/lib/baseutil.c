#include "baseutil.h"

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>

bool is_null_ptr(const void* ptr, const char* func_name) {
  if (ptr != NULL) return false;
  fprintf(stderr, _ERROR_SIG "function '%s' get null pointer\n", func_name);
  return true;
}

bool is_file_exist(const char* fname) {
  return access(fname, F_OK) == 0;
}

// [+|-][0123456789]
bool is_valid_digit_string(const char* str) {
  if (str == NULL || str[0] == '\0') return false;
  const char pos = '+';
  const char neg = '-';
  const char zero = '0';
  const char nine = '9';
  int idx = 1;
  // first char could be [+], [-], and [0~9]
  if (str[0] != pos && str[0] != neg && !(zero <= str[0] && str[0] <= nine)) {
    return false;
  }
  // the remaining could be [0~9]
  while (str[idx] != '\0') {
    if (!(zero <= str[idx] && str[idx] <= nine)){
      return false;
    } 
    idx += 1;
  }
  
  long val = strtol(str, NULL, 10);
  if ((val == LONG_MIN && errno == ERANGE) || val < -2147483648) { // underflow
    return false;
  }
  else if ((val == LONG_MAX && errno == ERANGE) || val > 2147483647) { // overflow
    return false;
  }

  return true;
}