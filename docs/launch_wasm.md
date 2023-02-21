# Launch Wasm Use WasmEdge C SDK

## Introduction
WasmEdge already provides `wasmedge` tool to run wasm applications. Therefore, I traced the [wasmedge tool implementation](https://github.com/WasmEdge/WasmEdge/blob/8573df4cf82f10546f35e6a0523b010ffcf90d31/lib/driver/runtimeTool.cpp), modified and simplified it with WasmEdge C SDK. To understand what is `_start` and `_initialize`, I checked up [WASI Application ABI](https://github.com/WebAssembly/WASI/blob/main/legacy/application-abi.md).

All the implementation are based on my previous work. To get more detail, please check [Launch Wasm Use WasmEdge C SDK (old)](./old_launch_wasm.md)

## Future Work
In `wasmedge` tool, user can set different options such as `--reactor` and `--enable-all`. This feature can be easily supported by using the Option struct and argument parser in this project. More details can be found in [add_option.md](./add_option.md).

There are few steps to implement the feature.

1. define options in handle_option_wasm_arg
2. run argument parser.
3. check if the options are found or not and execute corresponding program.

```cpp
bool handle_option_wasm_arg(const Option *opt) {
    // define options in handle_option_wasm_arg
    Option *Reactor = create_Option("--reactor", -1, handle_option_null);
    Option *PropAll = create_option("--enable-all", 0, handle_option_null);
    ...
    Option *define_opt[] = {
        Reactor,
        PropAll,
        ...
    }
    ParseData *pd = create_ParseData(define_opt, remain_arg, define_opt_len);

    // run argument parser
    handle_parse(pd, opt->args_len, opt->args);

    // check if the options are found or not and 
    // execute corresponding program.
    if (Reactor->found){
        ...
    }
    if (PropAll->found){
        ...
    }

```

By doing so, `./x run` would have subcommands `--reactor` and `--enable-all`. 