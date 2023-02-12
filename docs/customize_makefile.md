# Customize Makefile

## Introduction

The things can be customized

1. Compiler
2. Compile Flags
3. Build Directory
4. Execution Name
5. Location of libgtest.a

In Makefile, you can see the following option at the start and setup manually.

```make
############# CONFIG SECTION START #############
CC = clang
CXX = clang++

EXTRAFLAGS = 
CFLAGS = -Wall -Wextra -O2 $(EXTRAFLAGS)
CXXFLAGS = $(CFLAGS) -pthread
WASMFLAGS = -lwasmedge
GTEST = /usr/local/lib/libgtest.a

BUILD = ./src/build
TEST_BUILD = ./src/test_build

TARGET = x
TEST_TARGET = x_testall
############## CONFIG SECTION END ##############
```

For temporary setup, you can run in the following way:

```bash
make CC=gcc all
```

By doing so, the C compiler will be changed to gcc for once.
