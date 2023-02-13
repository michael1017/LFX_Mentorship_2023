FROM ubuntu:22.04

# Use Bash
SHELL ["/bin/bash", "-c"]

WORKDIR /tmp

# System Update, Install Dependency
RUN apt update -y && \ 
    apt install -y clang llvm make cmake git curl clang-format

# Rust Install
RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y --target wasm32-wasi

# GoogleTest Install
RUN git clone https://github.com/google/googletest.git -b v1.13.0 && \
    cd googletest && \
    cmake -DBUILD_GMOCK=OFF && \
    make -j && \
    make install

# WasmEdge Install
RUN curl -sSf https://raw.githubusercontent.com/WasmEdge/WasmEdge/master/utils/install.sh | bash

# Project Download
WORKDIR /
RUN git clone https://github.com/michael1017/LFX_Mentorship_2023.git app

# Project Build
WORKDIR /app
RUN source $HOME/.wasmedge/env && \
    source $HOME/.cargo/env && \
    make -j all test