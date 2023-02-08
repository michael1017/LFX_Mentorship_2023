# LFX Mentorship 2023

## Description

LFX Mentorship 2023 01-Mar-May Challenge - Wasmedge

## Getting Started

### Prerequirement
  
For Basic Install:
* Clang
* GNU Make
* WasmEdge Library

For Testing:  
Need to fulfill the basic install requirement, then install the following tools:
* Clang++
* llvm
* Google Test

### Installing

The project uses GNU Make system to build the source. Several things can be configured before starting to build. To learn more detail about configuration, please check [customize Makefile](docs/customize_makefile.md)

For Basic Install: 
By default, it will generate a execution file called `x`. To change the default execution name, please check [customize Makefile](docs/customize_makefile.md)
```
make all
```

Clean up object files and executions produced by `make all` and `make test`
```
make clean
```

### Executing
```
./x [version] [run] [wasm path] [arguments]
`./x` is the default execution name.  
`version` print wasmedge version
`run` is optional; expect the following arguments are `wasm path` and `arguments`.  
`wasm path` is required; the relative or absolute path of wasm app.  
`arguments` is optional; expect to be 32 bits signed integers (-2^31 ~ 2^31-1).
```
Example:
```
./x wasm_app/add.wasm 2 3 
./x run wasm_app/add.wasm 3 2 
./x version wasm_app/add.wasm 00100 -0
./x version run wasm_app/add.wasm +005 - 
```
![](docs/images/add.png)

### Testing

#### Run Google Test:
By default, it will generate a execution file called `x_testall`. To change the default execution name, please check [customize Makefile](docs/customize_makefile.md). The testing may print some error messages, this is because the test includes illegal test cases to check its error handling ability. Developers only need to check if "PASSED" is printed at the end of the tests.
```
make test
./x_testall
```

The "PASSED" message should be like this:
![](docs/images/test_pass.png)


Clean up object files and executions produced by `make test`
```
make clean_test
```

#### Run Clang Source-Based Code Coverage:
```
sh coverage_test.sh
```
The report should be like this:
![](docs/images/cov_report.png)


## Implementation Report

This section shows how I implemented the tool and provides screenshots of execution results.

* [How I implemented the tool](docs/implementation.md)
* [Screenshots of execution results](./docs/execution_results.md)

## Reference

* [WasmEdge](https://github.com/WasmEdge/WasmEdge)
* [WasmEdge C SDK](https://wasmedge.org/book/en/sdk/c.html)
* [GoogleTest](http://google.github.io/googletest)
* [Clang Source-Based Code Coverage](https://clang.llvm.org/docs/SourceBasedCodeCoverage.html)