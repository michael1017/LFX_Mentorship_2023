#include "baseutil.h"

#include <stdio.h>

bool is_null_ptr(const void* ptr, const char* func_name) {
  if (ptr != NULL) return false;
  fprintf(stderr, _BOLD_RED "ERROR =>" _RESET_COLOR \
    " function '%s' get null pointer\n", func_name);
  return true;
}