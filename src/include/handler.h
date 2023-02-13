#ifndef HANDLER_H
#define HANDLER_H

#include "argparse.h"
#include "option.h"

bool _handle_option_version(void);

bool _handle_option_wasm_arg(const Option *opt);

/// @brief Handle parsing result from ParseData.
/// @param pd is a ParseData pointer.
/// @return If exit successfully, return true. If not, return false.
bool handle_option(const ParseData *pd);

#endif /* HANDLER_H */