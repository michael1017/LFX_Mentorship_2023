# How To Add New Options

## Introduction

To add new options, there are three main steps:

1. Define Options
2. Create ParseData
3. Argument Parsing
4. Handle Options

By following the steps, programmer can:

1. easily add new options
2. create handlers for different options independently
3. Add new feature by modifying struct Option and ParseData.

This is the main function of the program, I will use it as an example in the following sections.

```cpp
int main(const int argc, const char **argv) {
  Option *define_opt[] = {create_Option("version", 0, handle_option_version),
                          create_Option("run", -1, handle_option_wasm_arg)};
  Option *remain_arg = create_Option(NULL, -1, handle_option_wasm_arg);
  int define_opt_len = sizeof(define_opt) / sizeof(Option *);
  ParseData *pd = create_ParseData(define_opt, remain_arg, define_opt_len);

  handle_parse(pd, argc, argv);
  handle_option(pd);

  delete_Option(remain_arg);
  delete_Option_array(define_opt, define_opt_len);
  delete_ParseData(pd);
  return 0;
}
```

## Define Option

Define option tells the program which options should be handled.

In the main function, we can see a function called `create_Option` inside `define_opt` array. 

1. The first argument of the `create_Option` is option's name.
2. The second is the expected argument length after the option.
3. The last is the function pointer of option handler. 

If the argument length is set to `-1`, it means that the option will accept all the arguments after it. To get more detail about `create_Option` please refer to [option.h File Reference](https://michael1017.github.io/LFX_Mentorship_2023/doxygen/html/option_8h.html)

For instance, to add a new option called "new_opt" with argument length equal to two, just simply add `create_Option("new_opt", 2, handle_option_null)` to the `define_opt` array. `handle_option_null` will do nothing for our "new_opt". User can create their own option handler which accepts a Option pointer and return a bool.

```cpp
Option *define_opt[] = {create_Option("version", 0, handle_option_version),
                        create_Option("run", -1, handle_option_wasm_arg),
                        create_Option("new_opt", 2, handle_option_null)};
Option *remain_arg = create_Option(NULL, -1, handle_option_wasm_arg);
```

Create a option handler

```cpp
bool handle_option_costume(Option *opt) {
  ...
  return _SUCCESS;
}
```

## Create ParseData
After we set up options, we need three parameters to create ParseData `pd`. Option `remain_arg` is used to handle arguments not belong to any options. To get more detail about `create_ParseData` please refer to [argparse.h File Reference](https://michael1017.github.io/LFX_Mentorship_2023/doxygen/html/argparse_8h.html)


```cpp
ParseData *pd = create_ParseData(define_opt, remain_arg, define_opt_len);
```

## Argument Parsing

The task of argument parsing is to find out the location of options in argv.

The program use `handle_parse` to take care of the task and return the result to `pd`. If the arguments are not belong to any options, they will be collected to `remain_arg`.

```cc
handle_parse(pd, argc, argv);
```

If the new option is correctly added to the `define_opt`, `handle_parse` should be able to get the arguments for the new option.

For instance, if we run the following command:  

```bash
./x version new_opt a b wasm_app/add.wasm 1 2
```

We can easily see that "new_opt" catches two arguments called "a" and "b". Since "run" option is not found, the arguments which should be wasm application's file path and its arguments are collected to `remain_arg`.
![](images/implement1.png)

## Handle Options

The task of handle options is to execute the option handler if the option is found in the provided arguments.

For instance, if "version" option is given, it will run the option handler `handle_option_version` and print out the wasm version.