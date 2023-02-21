# (Old) Launch Wasm Use WasmEdge C SDK

This document is used to describe the implementation in the [previous commit](https://github.com/michael1017/LFX_Mentorship_2023/blob/f2e81fb841cb1832f4477574cc2464a76baea45e/src/lib/handler.c)

Following are the main steps to use WasmEdge C SDK to run Wasm WASI applications.

1. Create VM
2. VM State Forward
3. Get Function Type Context
4. Get Param Type and Length
5. Module Instance Init WASI
6. Setup Wasm Params From ParseData
7. Execute
8. Get Return Type and Length
9. Get Return Data

## Create VM
Create a VM and add host registration to support WASI

```cc
WasmEdge_ConfigureContext *ConfCxt = WasmEdge_ConfigureCreate();
WasmEdge_ConfigureAddHostRegistration(ConfCxt, WasmEdge_HostRegistration_Wasi);
WasmEdge_VMContext *VMCxt = WasmEdge_VMCreate(ConfCxt, NULL);
```

## VM State Forward
The following graph explains the status of the `VM` context.

```
                       |========================|
              |------->|      VM: Initiated     |
              |        |========================|
              |                    |
              |                 LoadWasm
              |                    |
              |                    v
              |        |========================|
              |--------|       VM: Loaded       |<-------|
              |        |========================|        |
              |              |            ^              |
              |         Validate          |              |
          Cleanup            |          LoadWasm         |
              |              v            |            LoadWasm
              |        |========================|        |
              |--------|      VM: Validated     |        |
              |        |========================|        |
              |              |            ^              |
              |      Instantiate          |              |
              |              |          RegisterModule   |
              |              v            |              |
              |        |========================|        |
              |--------|    VM: Instantiated    |--------|
                       |========================|
                             |            ^
                             |            |
                             --------------
                Instantiate, Execute, ExecuteRegistered

```

We just created a new VM in the previous step, we need to run through the following steps before executing the VM.
1. LoadWasm
2. Validate
3. Instiate

```cpp
// May fail due to Wasm app not found
Res = WasmEdge_VMLoadWasmFromFile(VMCxt, opt->args[0]); 
WasmEdge_VMValidate(VMCxt);
WasmEdge_VMInstantiate(VMCxt);
```

## Get Function Type Context
Since we need to support various input from wasm application, we need know the function parameter and return type in advanced. The execution of Wasm will need to specify the entry point function "_start". Otherwise, the program will print error "function '_start' not found".

```cpp
WasmEdge_String FuncName = WasmEdge_StringCreateByCString("_start");
const WasmEdge_FunctionTypeContext* FuncType = WasmEdge_VMGetFunctionType(VMCxt, FuncName);
```

## Get Param Type and Length

After we get "function type context", we can get the type and length of function parameters and returns. In this step, we will compare the number of parameters and number of input arguments. If they are not the same and the number of parameters is not equal to zero, exit the program. 

The reason we need to do this check is because in [hello.rs](../wasm_app/wasm_hello/hello.rs), the main function would have no parameter. However, the main function takes the arguments passed to it. Therefore, we allow function with zero parameter continue running. Same implementation can be found in [wasmedge tool implementation](https://github.com/WasmEdge/WasmEdge/blob/8573df4cf82f10546f35e6a0523b010ffcf90d31/lib/driver/runtimeTool.cpp#L256).

```cpp
  uint32_t ParamLen = WasmEdge_FunctionTypeGetParametersLength(FuncType);
  if (ParamLen + 1 != (unsigned int)opt->args_len && ParamLen != 0) {
    fprintf(stderr, _ERROR_SIG "Mismatch ParamLen and ArgsLen\n");
    WasmEdge_ConfigureDelete(ConfCxt);
    WasmEdge_VMDelete(VMCxt);
    WasmEdge_StringDelete(FuncName);
    return _FAILED;
  }
  enum WasmEdge_ValType *ParamBuf = malloc(sizeof(enum WasmEdge_ValType) * ParamLen);
  WasmEdge_FunctionTypeGetParameters(FuncType, ParamBuf, ParamLen);
  WasmEdge_Value *Params = malloc(ParamLen * sizeof(WasmEdge_Value));
```

## Module Instance Init WASI
Without doing this step, the texts printed from `println!` in rust will not show on the screen.

```cpp
WasmEdge_ModuleInstanceInitWASI(ModCxt, (const char *const *)opt->args, opt->args_len, NULL, 0, NULL, 0);
```

## Setup Wasm Params From ParseData

Setup wasm parameters based on value types in ParamBuf. Now the program can support I32, I64, F32 and F64. V128 should be easily supported. However, we need to have a string to V128 convertor first.

```cpp
for (uint32_t i=0; i<ParamLen && i+1<(unsigned int)opt->args_len; i++) {
    if (ParamBuf[i] == WasmEdge_ValType_I32) {
        Params[i] = WasmEdge_ValueGenI32(atoi(opt->args[i+1]));
    } else if (ParamBuf[i] == WasmEdge_ValType_I64) {
        Params[i] = WasmEdge_ValueGenI64(atoll(opt->args[i+1]));
    } else if (ParamBuf[i] == WasmEdge_ValType_F32) {
        Params[i] = WasmEdge_ValueGenF32(atof(opt->args[i+1]));
    } else if (ParamBuf[i] == WasmEdge_ValType_F64) {
        Params[i] = WasmEdge_ValueGenF64(strtod(opt->args[i+1], NULL));
    }
    /// TODO: V128, NullRef, FuncRef and ExternRef
    /// TOOD: Create String to V128 Convertor
}
```

## Execute

We use asynchronous VM execution, although it doesn't bring significant performance improve.

```cpp
AsyncCxt = WasmEdge_VMAsyncExecute(VMCxt, FuncName, Params, ParamLen);

```

## Get Return Type and Length
Create buffers for receiving function return type and value.

```cpp
uint32_t ReturnLen = WasmEdge_FunctionTypeGetReturnsLength(FuncType);
enum WasmEdge_ValType *ReturnBuf = malloc(sizeof(enum WasmEdge_ValType) * ReturnLen);
WasmEdge_FunctionTypeGetReturns(FuncType, ReturnBuf, ReturnLen);
WasmEdge_Value *Returns = malloc(ReturnLen * sizeof(WasmEdge_Value));
```

## Get Return Data
Print return values based on value types in ReturnBuf. Now the program can support I32, I64, F32 and F64. V128 should be easily supported. However, we need to have a V128 printer first.

```cpp
Res = WasmEdge_AsyncGet(AsyncCxt, Returns, ReturnLen);
for (uint32_t i=0; i<ReturnLen; i++) {
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
```