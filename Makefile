CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I.

SRC_DIR = algorithms/source
BUILD_DIR = algorithms/build

# Get all .c files in source directory
ALGO_SOURCES = $(wildcard $(SRC_DIR)/*.c)
# Create executable names (remove .c extension)
ALGO_EXECS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%,$(ALGO_SOURCES))

all: scheduling_policies_executables

scheduling_policies_executables: $(ALGO_EXECS)

# Simple rule: each .c file becomes an executable
$(BUILD_DIR)/%: $(SRC_DIR)/%.c process.h
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean scheduling_policies_executables