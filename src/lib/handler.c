#include "handler.h"

#include "baseutil.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wasmedge/wasmedge.h>

bool handle_option_version(void) {
  printf("WasmEdge version: %s\n", WasmEdge_VersionGet());
  return _SUCCESS;
}

bool handle_option_wasm_arg(const Option *opt) {
  // Create VM
  WasmEdge_ConfigureContext *ConfCxt = WasmEdge_ConfigureCreate();
  WasmEdge_ConfigureAddHostRegistration(ConfCxt, WasmEdge_HostRegistration_Wasi);
  WasmEdge_VMContext *VMCxt = WasmEdge_VMCreate(ConfCxt, NULL);
  WasmEdge_ModuleInstanceContext *ModCxt =
      WasmEdge_VMGetImportModuleContext(VMCxt, WasmEdge_HostRegistration_Wasi);
  WasmEdge_Result Res;
  // WasmEdge_Async* AsyncCxt;

  // VM State Forward
  Res = WasmEdge_VMLoadWasmFromFile(VMCxt, opt->args[0]);
  if (!WasmEdge_ResultOK(Res)) {
    fprintf(stderr, _ERROR_SIG "%s\n", WasmEdge_ResultGetMessage(Res));
    WasmEdge_ConfigureDelete(ConfCxt);
    WasmEdge_VMDelete(VMCxt);
    return _FAILED;
  }
  WasmEdge_VMValidate(VMCxt);
  WasmEdge_VMInstantiate(VMCxt);

  // Get Function Type Context
  WasmEdge_String FuncName = WasmEdge_StringCreateByCString("_start");
  const WasmEdge_FunctionTypeContext *FuncType =
      WasmEdge_VMGetFunctionType(VMCxt, FuncName);

  // Get [Param, Return] Type and Length
  uint32_t ParamLen = WasmEdge_FunctionTypeGetParametersLength(FuncType);
  uint32_t ReturnLen = WasmEdge_FunctionTypeGetReturnsLength(FuncType);
  if (ParamLen + 1 != (unsigned int)opt->args_len && !(ParamLen == 0 && ReturnLen == 0)) {
    fprintf(stderr, _ERROR_SIG "Mismatch ParamLen and ArgsLen\n");
    WasmEdge_ConfigureDelete(ConfCxt);
    WasmEdge_VMDelete(VMCxt);
    WasmEdge_StringDelete(FuncName);
    return _FAILED;
  }
  enum WasmEdge_ValType *ParamBuf = malloc(sizeof(enum WasmEdge_ValType) * ParamLen);
  enum WasmEdge_ValType *ReturnBuf = malloc(sizeof(enum WasmEdge_ValType) * ReturnLen);
  WasmEdge_FunctionTypeGetParameters(FuncType, ParamBuf, ParamLen);
  WasmEdge_FunctionTypeGetReturns(FuncType, ReturnBuf, ReturnLen);
  WasmEdge_Value *Params = malloc(ParamLen * sizeof(WasmEdge_Value));
  WasmEdge_Value *Returns = malloc(ReturnLen * sizeof(WasmEdge_Value));

  // Init Wasi
  WasmEdge_ModuleInstanceInitWASI(ModCxt, (const char *const *)opt->args, opt->args_len,
                                  NULL, 0, NULL, 0);

  // Setup wasm Params from raw ParseData
  for (uint32_t i = 0; i < ParamLen && i + 1 < (unsigned int)opt->args_len; i++) {
    if (ParamBuf[i] == WasmEdge_ValType_I32) {
      Params[i] = WasmEdge_ValueGenI32(atoi(opt->args[i + 1]));
    } else if (ParamBuf[i] == WasmEdge_ValType_I64) {
      Params[i] = WasmEdge_ValueGenI64(atoll(opt->args[i + 1]));
    } else if (ParamBuf[i] == WasmEdge_ValType_F32) {
      Params[i] = WasmEdge_ValueGenF32(atof(opt->args[i + 1]));
    } else if (ParamBuf[i] == WasmEdge_ValType_F64) {
      Params[i] = WasmEdge_ValueGenF64(strtod(opt->args[i + 1], NULL));
    }
    /// TODO: V128, NullRef, FuncRef and ExternRef
    /// TOOD: Create String to V128 Convertor
  }

  // Execute
  Res = WasmEdge_VMExecute(VMCxt, FuncName, Params, ParamLen, Returns, ReturnLen);
  // AsyncCxt = WasmEdge_VMAsyncExecute(VMCxt, FuncName, Params, ParamLen);
  // Res = WasmEdge_AsyncGet(AsyncCxt, Returns, ReturnLen);

  // Get Return Data
  for (uint32_t i = 0; i < ReturnLen; i++) {
    if (ReturnBuf[i] == WasmEdge_ValType_I32) {
      printf("%d\n", WasmEdge_ValueGetI32(Returns[i]));
    } else if (ReturnBuf[i] == WasmEdge_ValType_I64) {
      printf("%ld\n", WasmEdge_ValueGetI64(Returns[i]));
    } else if (ReturnBuf[i] == WasmEdge_ValType_F32) {
      printf("%f\n", WasmEdge_ValueGetF32(Returns[i]));
    } else if (ReturnBuf[i] == WasmEdge_ValType_F64) {
      printf("%lf\n", WasmEdge_ValueGetF64(Returns[i]));
    }
    /// TODO: V128, NullRef, FuncRef and ExternRef
    /// TOOD: Create V128 printer
  }

  /* Resources deallocations. */
  WasmEdge_ConfigureDelete(ConfCxt);
  WasmEdge_VMDelete(VMCxt);
  WasmEdge_StringDelete(FuncName);
  free(ParamBuf);
  free(ReturnBuf);
  free(Params);
  free(Returns);

  if (!WasmEdge_ResultOK(Res)) {
    fprintf(stderr, _ERROR_SIG "%s\n", WasmEdge_ResultGetMessage(Res));
    return _FAILED;
  }
  return _SUCCESS;
}

bool handle_option(const ParseData *pd) {
  // Handle options' argument
  int opt_run_idx = -1;
  bool state;
  for (int i = 0; i < pd->opt_len; i++) {
    if (pd->opt[i]->found == false)
      continue;

    if (strcmp("version", pd->opt[i]->opt_name) == 0) { // Option version
      state = handle_option_version();
      if (state == _FAILED) {
        fprintf(stderr, _ERROR_SIG "%s: handle_option_version failed\n", __func__);
        return _FAILED;
      }
    } else if (strcmp("run", pd->opt[i]->opt_name) == 0) { // Option run
      // handle with remain arg
      opt_run_idx = i;
    }
  }

  // run wasm app
  if (opt_run_idx != -1) {
    state = handle_option_wasm_arg(pd->opt[opt_run_idx]);
  } else if (pd->remain_arg->found == true) { // expect to be wasm arg
    state = handle_option_wasm_arg(pd->remain_arg);
  }
  if (state == _FAILED) {
    fprintf(stderr, _ERROR_SIG "%s: handle_option_wasm_arg failed\n", __func__);
    return _FAILED;
  }

  return _SUCCESS;
}