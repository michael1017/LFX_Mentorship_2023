############# CONFIG SECTION START #############
CC = clang
CXX = clang++

EXTRAFLAGS = -fprofile-instr-generate -fcoverage-mapping
CFLAGS = -Wall -Wextra -O2 $(EXTRAFLAGS)
CXXFLAGS = $(CFLAGS) -pthread
WASMFLAGS = -lwasmedge
GTEST = /usr/local/lib/libgtest.a

SRC_PREFIX = ./src
MAIN_FILE = ./src/main.c
BUILD = ./src/build
TEST_BUILD = ./src/test_build

TARGET = x
TEST_TARGET = x_testall
############## CONFIG SECTION END ##############

INCLUDE = $(SRC_PREFIX)/include
LIB = $(SRC_PREFIX)/lib
TEST = $(SRC_PREFIX)/test

MAIN_OBJ = $(BUILD)/main.o

LIB_FILE := $(wildcard $(LIB)/*.c)
OBJECT_FILE := $(patsubst $(LIB)/%.c, $(BUILD)/%.o, $(LIB_FILE))

TEST_FILE := $(wildcard $(TEST)/*.cc)
TEST_OBJECT:= $(patsubst $(TEST)/%.cc, $(TEST_BUILD)/%.o, $(TEST_FILE))

.PHONY: all test clean clean_test

all: $(OBJECT_FILE) $(MAIN_OBJ)
	$(CC) $(CFLAGS) $(WASMFLAGS) -I$(INCLUDE) $^ -o $(TARGET)

test: $(OBJECT_FILE) $(TEST_OBJECT) $(GTEST)
	$(CXX) $(CXXFLAGS) $(WASMFLAGS) -I$(INCLUDE) $^ -o $(TEST_TARGET) 

# Build ./test/*.cc, require {"./test/*.cc", "./test_build"}
$(TEST_BUILD)/%.o: $(TEST)/%.cc $(TEST_BUILD)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE) -c $< -o $@

# Build main.c, require {main.c}
$(MAIN_OBJ): $(MAIN_FILE)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

# Build ./lib/*.c, require {"./lib/*.c", "./include/*.h", "./build"}
$(BUILD)/%.o: $(LIB)/%.c $(INCLUDE)/%.h $(BUILD)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

# Make ./build dir if non-exist
$(BUILD):
	mkdir -p $(BUILD)

# Make ./test_build dir if non-exist
$(TEST_BUILD):
	mkdir -p $(TEST_BUILD)

clean:
	rm $(OBJECT_FILE)
	rm $(TARGET)
	make clean_test

clean_test:
	rm $(TEST_OBJECT)
	rm $(TEST_TARGET)