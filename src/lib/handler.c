#include "handler.h"

#include "baseutil.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wasmedge/wasmedge.h>

bool handle_option_null(const Option *opt) {
  // do nothing
  return _SUCCESS;
}

bool handle_option_version(const Option *opt) {
  printf("WasmEdge version: %s\n", WasmEdge_VersionGet());
  return _SUCCESS;
}

bool handle_option_wasm_arg(const Option *opt) {
  if (opt->args_len == 0) {
    fprintf(stderr, _ERROR_SIG "Wasm file path is needed\n");
    return _FAILED;
  }

  // Create VM
  WasmEdge_ConfigureContext *ConfCxt = WasmEdge_ConfigureCreate();
  WasmEdge_ConfigureAddHostRegistration(ConfCxt, WasmEdge_HostRegistration_Wasi);
  WasmEdge_VMContext *VMCxt = WasmEdge_VMCreate(ConfCxt, NULL);
  WasmEdge_ModuleInstanceContext *ModCxt =
      WasmEdge_VMGetImportModuleContext(VMCxt, WasmEdge_HostRegistration_Wasi);
  WasmEdge_Result Res;
  // remain ConfCxt, VMCxt

  // VM State Forward
  Res = WasmEdge_VMLoadWasmFromFile(VMCxt, opt->args[0]);
  if (!WasmEdge_ResultOK(Res)) {
    fprintf(stderr, _ERROR_SIG "%s\n", WasmEdge_ResultGetMessage(Res));
    WasmEdge_ConfigureDelete(ConfCxt);
    WasmEdge_VMDelete(VMCxt);
    return _FAILED;
  }
  Res = WasmEdge_VMValidate(VMCxt);
  if (!WasmEdge_ResultOK(Res)) {
    fprintf(stderr, _ERROR_SIG "%s\n", WasmEdge_ResultGetMessage(Res));
    WasmEdge_ConfigureDelete(ConfCxt);
    WasmEdge_VMDelete(VMCxt);
    return _FAILED;
  }
  Res = WasmEdge_VMInstantiate(VMCxt);
  if (!WasmEdge_ResultOK(Res)) {
    fprintf(stderr, _ERROR_SIG "%s\n", WasmEdge_ResultGetMessage(Res));
    WasmEdge_ConfigureDelete(ConfCxt);
    WasmEdge_VMDelete(VMCxt);
    return _FAILED;
  }
  // N/A

  // Get Function Type Context
  WasmEdge_String StartFunc = WasmEdge_StringCreateByCString("_start");
  WasmEdge_String InitFunc = WasmEdge_StringCreateByCString("_initialize");
  int FuncListLen = WasmEdge_VMGetFunctionListLength(VMCxt);
  WasmEdge_String *FuncNames = malloc(sizeof(WasmEdge_String) * FuncListLen);
  const WasmEdge_FunctionTypeContext **FuncTypes =
      malloc(sizeof(WasmEdge_FunctionTypeContext *) * FuncListLen);
  // Remain FuncNames, FuncTypes, StartFunc, InitFunc

  // Check Command Mode
  bool HasStartFunc = false;
  bool HasInitFunc = false;
  WasmEdge_String FuncName;
  WasmEdge_FunctionTypeContext *FuncType;
  WasmEdge_VMGetFunctionList(VMCxt, FuncNames, FuncTypes, FuncListLen);
  for (int i = 0; i < FuncListLen; i++) {
    if (WasmEdge_StringIsEqual(FuncNames[i], StartFunc)) {
      HasStartFunc = true;
      FuncName = FuncNames[i];
      FuncType = (WasmEdge_FunctionTypeContext *)FuncTypes[i];
    } else if (WasmEdge_StringIsEqual(FuncNames[i], InitFunc)) {
      HasInitFunc = true;
    }
  }
  int ParamLen = WasmEdge_FunctionTypeGetParametersLength(FuncType);
  int ReturnLen = WasmEdge_FunctionTypeGetReturnsLength(FuncType);
  bool Valid = (ParamLen == 0 && ReturnLen == 0);
  bool EnterCommandMode = HasStartFunc && Valid;
  // N/A

  // Init Wasi
  WasmEdge_ModuleInstanceInitWASI(ModCxt, (const char *const *)opt->args, opt->args_len,
                                  NULL, 0, NULL, 0);
  // N/A

  // Enter Command Mode
  if (EnterCommandMode) {
    Res = WasmEdge_VMExecute(VMCxt, StartFunc, NULL, 0, NULL, 0);
    WasmEdge_StringDelete(StartFunc);
    WasmEdge_StringDelete(InitFunc);
    free(FuncNames);
    free(FuncTypes);
    WasmEdge_ConfigureDelete(ConfCxt);
    WasmEdge_VMDelete(VMCxt);
    if (!WasmEdge_ResultOK(Res)) {
      fprintf(stderr, _ERROR_SIG "%s\n", WasmEdge_ResultGetMessage(Res));
      return _FAILED;
    }
    return _SUCCESS;
  } else { // Enter Reactor Mode
    if (HasInitFunc) {
      Res = WasmEdge_VMExecute(VMCxt, InitFunc, NULL, 0, NULL, 0);
      if (!WasmEdge_ResultOK(Res)) {
        WasmEdge_StringDelete(StartFunc);
        WasmEdge_StringDelete(InitFunc);
        free(FuncNames);
        free(FuncTypes);
        WasmEdge_ConfigureDelete(ConfCxt);
        WasmEdge_VMDelete(VMCxt);
        fprintf(stderr, _ERROR_SIG "%s\n", WasmEdge_ResultGetMessage(Res));
        return _FAILED;
      }
    }

    // Setup wasm Params from raw ParseData
    const int ArgsLen = opt->args_len - 1;
    enum WasmEdge_ValType *ParamBuf = malloc(sizeof(enum WasmEdge_ValType) * ArgsLen);
    WasmEdge_Value *Params = malloc(ArgsLen * sizeof(WasmEdge_Value));
    WasmEdge_FunctionTypeGetParameters(FuncType, ParamBuf, ArgsLen);
    printf("hello\n");
    for (int i = 0; i < ParamLen && i < ArgsLen; i++) {
      if (ParamBuf[i] == WasmEdge_ValType_I32) {
        Params[i] = WasmEdge_ValueGenI32(atoi(opt->args[i + 1]));
      } else if (ParamBuf[i] == WasmEdge_ValType_I64) {
        Params[i] = WasmEdge_ValueGenI64(atoll(opt->args[i + 1]));
      } else if (ParamBuf[i] == WasmEdge_ValType_F32) {
        Params[i] = WasmEdge_ValueGenF32((float)atof(opt->args[i + 1]));
      } else if (ParamBuf[i] == WasmEdge_ValType_F64) {
        Params[i] = WasmEdge_ValueGenF64(atof(opt->args[i + 1]));
      }
      /// TODO: V128, NullRef, FuncRef and ExternRef
      /// TOOD: Create String to V128 Convertor
    }
    if (ParamLen < ArgsLen) {
      for (int i = ParamLen; i < ArgsLen; i++) {
        // Follow the original design in wasmedge
        const uint64_t Value = (uint64_t)atoll(opt->args[i + 1]);
        Params[i] = WasmEdge_ValueGenI64(Value);
        ParamBuf[i] = WasmEdge_ValType_F64;
      }
    }
    // Remain ParamBuf, Params

    // Execute
    WasmEdge_Async *AsyncCxt = WasmEdge_VMAsyncExecute(VMCxt, FuncName, Params, ArgsLen);
    // Remain AsyncCxt

    // Get Return Data
    enum WasmEdge_ValType *ReturnBuf = malloc(sizeof(enum WasmEdge_ValType) * ReturnLen);
    WasmEdge_FunctionTypeGetReturns(FuncType, ReturnBuf, ReturnLen);
    WasmEdge_Value *Returns = malloc(ReturnLen * sizeof(WasmEdge_Value));
    Res = WasmEdge_AsyncGet(AsyncCxt, Returns, ReturnLen);
    if (!WasmEdge_ResultOK(Res)) {
      fprintf(stderr, _ERROR_SIG "%s\n", WasmEdge_ResultGetMessage(Res));
      WasmEdge_StringDelete(StartFunc);
      WasmEdge_StringDelete(InitFunc);
      free(FuncNames);
      free(FuncTypes);
      WasmEdge_AsyncDelete(AsyncCxt);
      WasmEdge_ConfigureDelete(ConfCxt);
      WasmEdge_VMDelete(VMCxt);
      free(ParamBuf);
      free(ReturnBuf);
      free(Params);
      free(Returns);
      return _FAILED;
    }
    for (int i = 0; i < ReturnLen; i++) {
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
    // Remain ReturnBuf, Returns

    /* Resources deallocations. */
    WasmEdge_StringDelete(StartFunc);
    WasmEdge_StringDelete(InitFunc);
    free(FuncNames);
    free(FuncTypes);
    WasmEdge_AsyncDelete(AsyncCxt);
    WasmEdge_ConfigureDelete(ConfCxt);
    WasmEdge_VMDelete(VMCxt);
    free(ParamBuf);
    free(ReturnBuf);
    free(Params);
    free(Returns);
    return _SUCCESS;
  }
}

bool handle_option(const ParseData *pd) {
  // Handle options' argument
  bool state;
  for (int i = 0; i < pd->opt_len; i++) {
    if (pd->opt[i]->found == false)
      continue;

    state = pd->opt[i]->handle_func(pd->opt[i]);
    if (state == _FAILED) {
      fprintf(stderr, _ERROR_SIG "%s: Option '%s' handle_func failed\n", __func__,
              pd->opt[i]->opt_name);
      return _FAILED;
    }
  }

  // run wasm app
  if (pd->remain_arg->found == true) {
    state = handle_option_wasm_arg(pd->remain_arg);
  }
  if (state == _FAILED) {
    fprintf(stderr, _ERROR_SIG "%s: remain_arg handle_func failed\n", __func__);
    return _FAILED;
  }

  return _SUCCESS;
}