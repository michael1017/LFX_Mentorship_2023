############# CONFIG SECTION START #############
CC = clang
CFLAGS = -Wall -Wextra -O2
WASMFLAGS = -lwasmedge

SRC_PREFIX = ./src
MAIN_FILE = ./src/main.c
BUILD = ./src/build

TARGET = x
############## CONFIG SECTION END ##############

INCLUDE = $(SRC_PREFIX)/include
LIB = $(SRC_PREFIX)/lib

MAIN_OBJ = $(BUILD)/main.o

LIB_FILE := $(wildcard $(LIB)/*.c)
OBJECT_FILE := $(patsubst $(LIB)/%.c, $(BUILD)/%.o, $(LIB_FILE))
OBJECT_FILE += $(MAIN_OBJ)

.PHONY: all clean

all: $(OBJECT_FILE)
	$(CC) $(CFLAGS) $(WASMFLAGS) -I$(INCLUDE) $^ -o $(TARGET)

# Build main.c, require {main.c}
$(MAIN_OBJ): $(MAIN_FILE)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

# Build ./lib/*.c, require {"./lib/*.c", "./include/*.h", "./build"}
$(BUILD)/%.o: $(LIB)/%.c $(INCLUDE)/%.h $(BUILD)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

# Make ./build dir if non-exist
$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm $(OBJECT_FILE)
	rm $(TARGET)