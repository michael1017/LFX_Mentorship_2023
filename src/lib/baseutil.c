#include "baseutil.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

bool is_null_ptr(const void *ptr, const char *func_name) {
  if (ptr != NULL)
    return false;
  fprintf(stderr, _ERROR_SIG "function '%s' get null pointer\n", func_name);
  return true;
}
