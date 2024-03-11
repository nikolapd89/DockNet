# Compiler settings - Can override with environment variables.
CC ?= clang
CFLAGS = -Iinclude -Wall -Wextra -pedantic -O2

# Linker settings
LDFLAGS =

# Source and object files
SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:src/%.c=build/%.o)

# Output binary
TARGET = bin/docknet

# Create build and bin directories
BUILD_DIR = build
BIN_DIR = bin

# Default target
all: $(BUILD_DIR) $(BIN_DIR) $(TARGET)

# Ensure build and bin directories exist
$(BUILD_DIR) $(BIN_DIR):
	@mkdir -p $@

# Linking the target binary
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Compiling source files to object files
build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

# Phony targets for commands that do not represent files
.PHONY: all clean
