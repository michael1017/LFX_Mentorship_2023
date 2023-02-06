extern "C" {
    #include "handler.h"

    #include "baseutil.h"
}
#include <stdbool.h>
#include <wasmedge/wasmedge.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtest/gtest.h>

TEST(handler, is_valid_wasm_arg) {
    
    Option* opt = create_Option(NULL, -1);
    opt->found = true;
    {
        char *args[] = {(char *)"/dev/null", (char *)"-0"};
        opt->args_len = sizeof(args) / sizeof(char*);
        opt->args = args;

        EXPECT_EQ(is_valid_wasm_arg(opt), true);
    }
    {
        char *args[] = {(char *)"/dev/null/foo", (char *)"0123456789"};
        opt->args_len = sizeof(args) / sizeof(char*);
        opt->args = args;

        EXPECT_EQ(is_valid_wasm_arg(opt), false);
    }
    {
        char *args[] = {(char *)"/dev/null", (char *)"a"};
        opt->args_len = sizeof(args) / sizeof(char*);
        opt->args = args;

        EXPECT_EQ(is_valid_wasm_arg(opt), false);
    }
    delete_Option(opt);
}

TEST(handler, handle_option_version) {
    EXPECT_EQ(handle_option_version(), _SUCCESS);
}

TEST(handler, handle_option_wasm_arg) {
    // The Test may fail due to wasm file non-exist
    Option* opt = create_Option(NULL, -1);
    opt->found = true;
    {
        char *args[] = {(char *)"wasm_app/add.wasm", (char *)"1111", (char *)"2222"};
        opt->args_len = sizeof(args) / sizeof(char*);
        opt->args = args;

        EXPECT_EQ(handle_option_wasm_arg(opt), _SUCCESS);
    }
    {
        char *args[] = {(char *)"wasm_app/factorial.wasm", (char *)"5"};
        opt->args_len = sizeof(args) / sizeof(char*);
        opt->args = args;

        EXPECT_EQ(handle_option_wasm_arg(opt), _SUCCESS);
    }
    {
        char *args[] = {(char *)"wasm_app/fibonacci.wasm", (char *)"5"};
        opt->args_len = sizeof(args) / sizeof(char*);
        opt->args = args;

        EXPECT_EQ(handle_option_wasm_arg(opt), _SUCCESS);
    }
    delete_Option(opt);

}

TEST(handler, handle_option) {
    // The Test may fail due to wasm file non-exist
    {
        ParseData* pd = create_ParseData();
        Option* define_opt[] = {
            create_Option((char* )"version", 0), 
            create_Option((char* )"run", -1)
        };
        int define_opt_len = sizeof(define_opt)/sizeof(Option*);
        const char *argv[] = {"./execution", "version", "run", "wasm_app/add.wasm", "-22222", "11111"};
        const int argc = sizeof(argv) / sizeof(char*);

        handle_parse(pd, define_opt, define_opt_len, argc, argv);
        EXPECT_EQ(handle_option(pd), _SUCCESS);

        delete_Option_array(define_opt, define_opt_len);
        delete_ParseData(pd);
    }
    {
        ParseData* pd = create_ParseData();
        Option* define_opt[] = {
            create_Option((char* )"version", 0), 
            create_Option((char* )"run", -1)
        };
        int define_opt_len = sizeof(define_opt)/sizeof(Option*);
        const char *argv[] = {"./execution", "version", "wasm_app/add.wasm", "-22222", "11111"};
        const int argc = sizeof(argv) / sizeof(char*);

        handle_parse(pd, define_opt, define_opt_len, argc, argv);
        EXPECT_EQ(handle_option(pd), _SUCCESS);

        delete_Option_array(define_opt, define_opt_len);
        delete_ParseData(pd);
    }
        {
        ParseData* pd = create_ParseData();
        Option* define_opt[] = {
            create_Option((char* )"version", 0), 
            create_Option((char* )"run", -1)
        };
        int define_opt_len = sizeof(define_opt)/sizeof(Option*);
        const char *argv[] = {"./execution", "version", "wasm_app/add.wasm", "-22222", "11111", "123"};
        const int argc = sizeof(argv) / sizeof(char*);

        handle_parse(pd, define_opt, define_opt_len, argc, argv);
        EXPECT_EQ(handle_option(pd), _FAILED);

        delete_Option_array(define_opt, define_opt_len);
        delete_ParseData(pd);
    }

}