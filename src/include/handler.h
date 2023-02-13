#ifndef HANDLER_H
#define HANDLER_H

#include "argparse.h"
#include "option.h"

bool handle_option_version(void);
bool handle_option_wasm_arg(const Option *);
bool handle_option(const ParseData *);

#endif /* HANDLER_H */