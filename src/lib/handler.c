#include "handler.h"

#include "baseutil.h"

#include <wasmedge/wasmedge.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool is_valid_wasm_arg(const Option* opt) {
  bool state;
  state = is_file_exist(opt->args[0]);
  if (state == false){
    fprintf(stderr, _ERROR_SIG "%s: the file '%s' not exist\n", __func__, opt->args[0]);
    return false;
  } 

  for (int i=1; i<opt->args_len; i++) {
    state = is_valid_digit_string(opt->args[i]); // [+|-][0-9]
    if (state == false) {
      fprintf(stderr, _ERROR_SIG "%s: arg '%s' is not valid digit string\n", __func__, opt->args[i]);
      return false;
    } 
  }

  return true;
}

bool handle_option_version(void) {
  printf("WasmEdge version: %s\n", WasmEdge_VersionGet());
  return _SUCCESS;
}

bool handle_option_wasm_arg(const Option* opt) {
    bool state;
    state = is_valid_wasm_arg(opt);
    if (state == false){
      fprintf(stderr, _ERROR_SIG "%s: invalid wasm arg\n", __func__);
      show_opt(opt);
      return _FAILED;
    } 

    //////////////////////////////// Disallow Return Start ////////////////////////////////
    WasmEdge_ConfigureContext *ConfCxt = WasmEdge_ConfigureCreate();
    WasmEdge_ConfigureAddHostRegistration(ConfCxt, WasmEdge_HostRegistration_Wasi);
    WasmEdge_VMContext *VMCxt = WasmEdge_VMCreate(ConfCxt, NULL);

    // Low Buffer Size
    // Since we just want the first function
    const uint32_t BUF_LEN = 1;
    WasmEdge_String FuncNames[BUF_LEN];
    WasmEdge_VMLoadWasmFromFile(VMCxt, opt->args[0]);
    WasmEdge_VMValidate(VMCxt);
    WasmEdge_VMInstantiate(VMCxt);

    uint32_t RealFuncNum = WasmEdge_VMGetFunctionList(VMCxt, FuncNames, NULL, BUF_LEN);
    if (RealFuncNum > 1) {
      printf(_WARNING_SIG "The program only excute the first function in .wasm file\n");
      printf(_WARNING_SIG "Detected %u functions in %s file\n", RealFuncNum, opt->args[0]);
      // TODO: show function name
    }


    // Setup wasm params and return
    int Params_len = opt->args_len-1;
    WasmEdge_Value* Params = malloc(Params_len * sizeof(WasmEdge_Value));
    WasmEdge_Value Returns[1];
    for (int i=0; i<Params_len; i++) {
      Params[i] = WasmEdge_ValueGenI32(atoi(opt->args[i+1]));
    }

    WasmEdge_Result Res = WasmEdge_VMExecute(VMCxt, FuncNames[0], Params, Params_len, Returns, 1);


    /* Resources deallocations. */
    free(Params);
    WasmEdge_VMDelete(VMCxt);
    WasmEdge_ConfigureDelete(ConfCxt);
    //////////////////////////////// Disallow Return End ////////////////////////////////

    if (!WasmEdge_ResultOK(Res)) {
      fprintf(stderr, _ERROR_SIG "%s\n", WasmEdge_ResultGetMessage(Res));
      return _FAILED;
    }

    printf("Get result: %d\n", WasmEdge_ValueGetI32(Returns[0]));
    return _SUCCESS;
}

bool handle_option(const ParseData* pd) {
  // Handle options' argument
  int opt_run_idx = -1;
  bool state;
  for (int i=0; i<pd->opt_len; i++) {
    if (pd->opt[i]->found == false) continue; 

    if (strcmp("version", pd->opt[i]->opt_name) == 0) { // Option version
      state = handle_option_version();
      if (state == _FAILED){
        fprintf(stderr, _ERROR_SIG "%s: handle_option_version failed\n", __func__);
        return _FAILED;
      }
    }
    else if (strcmp("run", pd->opt[i]->opt_name) == 0) { // Option run
      // handle with remain arg
      opt_run_idx = i;
    }
  }

  // run wasm app
  if (opt_run_idx != -1) {
    state = handle_option_wasm_arg(pd->opt[opt_run_idx]);
  }
  else if (pd->remain_arg->found == true) { // expect to be wasm arg
    state = handle_option_wasm_arg(pd->remain_arg);
  }
  if (state == _FAILED){
    fprintf(stderr, _ERROR_SIG "%s: handle_option_version failed\n", __func__);
    return _FAILED;
  }

  return _SUCCESS;
}