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

  return true;
}

bool handle_option_version(void) {
  printf("WasmEdge version: %s\n", WasmEdge_VersionGet());
  return _SUCCESS;
}

bool handle_option_wasm_arg(const Option* opt) {
    bool state;
    bool error_flag = false;
    state = is_valid_wasm_arg(opt);
    if (state == false){
      fprintf(stderr, _ERROR_SIG "%s: invalid wasm arg\n", __func__);
      show_opt(opt);
      return _FAILED;
    } 
    //////////////////////////////// Disallow Return Start ////////////////////////////////
    // Create VM
    WasmEdge_ConfigureContext *ConfCxt = WasmEdge_ConfigureCreate();
    WasmEdge_ConfigureAddHostRegistration(ConfCxt, WasmEdge_HostRegistration_Wasi);
    WasmEdge_VMContext *VMCxt = WasmEdge_VMCreate(ConfCxt, NULL);
    
    // VM State Forward
    WasmEdge_VMLoadWasmFromFile(VMCxt, opt->args[0]);
    WasmEdge_VMValidate(VMCxt);
    WasmEdge_VMInstantiate(VMCxt);

    // Declair Variables
    WasmEdge_Result Res;
    enum WasmEdge_ValType* ParamBuf = NULL;
    enum WasmEdge_ValType* ReturnBuf = NULL;
    const uint32_t BUF_LEN = 1; // Accept One
    WasmEdge_String FuncName;

    // Get Function Type Context
    uint32_t RealFuncNum = WasmEdge_VMGetFunctionList(VMCxt, &FuncName, NULL, BUF_LEN);
    if (RealFuncNum > 1) {
      FuncName = WasmEdge_StringCreateByCString("_start");
    }
    const WasmEdge_FunctionTypeContext* FuncType = WasmEdge_VMGetFunctionType(VMCxt, FuncName);

    // Get [Param, Return] Type and Length
    uint32_t ParamLen = WasmEdge_FunctionTypeGetParametersLength(FuncType);
    uint32_t ReturnLen = WasmEdge_FunctionTypeGetReturnsLength(FuncType);
    ParamBuf = malloc(sizeof(enum WasmEdge_ValType) * ParamLen);
    ReturnBuf = malloc(sizeof(enum WasmEdge_ValType) * ReturnLen);
    WasmEdge_FunctionTypeGetParameters(FuncType, ParamBuf, ParamLen);
    WasmEdge_FunctionTypeGetReturns(FuncType, ReturnBuf, ReturnLen);
    WasmEdge_Value* Params = malloc(ParamLen * sizeof(WasmEdge_Value));
    WasmEdge_Value* Returns = malloc(ReturnLen * sizeof(WasmEdge_Value));
    
    // Setup wasm Params and Returns from raw ParseData
    if (ParamLen == (unsigned int)opt->args_len-1) { // Continue if same param_len
      // Process Params
      for (uint32_t i=0; i<ParamLen; i++) {
        if (ParamBuf[i] == WasmEdge_ValType_I32) {
          printf("WasmEdge_ValType_I32\n");
          Params[i] = WasmEdge_ValueGenI32(atoi(opt->args[i+1]));
        } else if (ParamBuf[i] == WasmEdge_ValType_I64) {
          printf("WasmEdge_ValType_I64\n");
          Params[i] = WasmEdge_ValueGenI64(atoll(opt->args[i+1]));
        } else if (ParamBuf[i] == WasmEdge_ValType_F32) {
          printf("WasmEdge_ValType_F32\n");
          Params[i] = WasmEdge_ValueGenF32(atof(opt->args[i+1]));
        } else if (ParamBuf[i] == WasmEdge_ValType_F64) {
          printf("WasmEdge_ValType_F64\n");
          Params[i] = WasmEdge_ValueGenF64(strtod(opt->args[i+1], NULL));
        }
        /// TODO: V128, NullRef, FuncRef and ExternRef
        /// TOOD: Create String to V128 Convertor
      }
      
      Res = WasmEdge_VMExecute(VMCxt, FuncName, Params, ParamLen, Returns, ReturnLen);
    }
    else {
      error_flag = true;
      fprintf(stderr, _ERROR_SIG "argument num, parameter length => Mismatch\n");
      fprintf(stderr, _ERROR_SIG "argument num: %u\n", (unsigned int)opt->args_len-1);
      fprintf(stderr, _ERROR_SIG "parameter length: %u\n", ParamLen);
    }
      
    for (uint32_t i=0; i<ReturnLen; i++) {
      if (ReturnBuf[i] == WasmEdge_ValType_I32) {
        printf("WasmEdge_ValType_I32\n");
        printf("%d\n", WasmEdge_ValueGetI32(Returns[i]));
      } else if (ReturnBuf[i] == WasmEdge_ValType_I64) {
        printf("WasmEdge_ValType_I64\n");
        printf("%ld\n", WasmEdge_ValueGetI64(Returns[i]));
      } else if (ReturnBuf[i] == WasmEdge_ValType_F32) {
        printf("WasmEdge_ValType_F32\n");
        printf("%f\n", WasmEdge_ValueGetF32(Returns[i]));
      } else if (ReturnBuf[i] == WasmEdge_ValType_F64) {
        printf("WasmEdge_ValType_F64\n");
        printf("%lf\n", WasmEdge_ValueGetF64(Returns[i]));
      }
      /// TODO: V128, NullRef, FuncRef and ExternRef
      /// TOOD: Create V128 printer
    }

    /* Resources deallocations. */
    WasmEdge_ConfigureDelete(ConfCxt);
    WasmEdge_VMDelete(VMCxt);
    if (RealFuncNum > 1) WasmEdge_StringDelete(FuncName);
    free(ParamBuf);
    free(ReturnBuf);
    free(Params);
    free(Returns);
    //////////////////////////////// Disallow Return End ////////////////////////////////  
  

    if (error_flag == true) {
      return _FAILED;
    }
    else if (!WasmEdge_ResultOK(Res)) {
      fprintf(stderr, _ERROR_SIG "%s\n", WasmEdge_ResultGetMessage(Res));
      return _FAILED;
    }
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
    fprintf(stderr, _ERROR_SIG "%s: handle_option_wasm_arg failed\n", __func__);
    return _FAILED;
  }

  return _SUCCESS;
}