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