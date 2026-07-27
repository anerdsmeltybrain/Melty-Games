# Project settings
TARGET = cactus_run
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic

# Raylib & System Libraries
RAYLIB_FLAGS = $(shell pkg-config --cflags --libs raylib 2>/dev/null || echo "-lraylib -lGL -lm -lpthread -ldl -lrt -lX11")

# Source files
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

.PHONY: all run debug release clean re help

# Default target
all: $(TARGET)

# Build executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(RAYLIB_FLAGS)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile and run the game
run: $(TARGET)
	./$(TARGET)

# Debug build mode
debug: CFLAGS += -g -DDEBUG
debug: re

# Release build mode
release: CFLAGS += -O2 -s
release: re

# Rebuild target
re: clean all

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Show help menu
help:
	@echo "Cactus Run Makefile targets:"
	@echo "  make         - Build the game"
	@echo "  make run     - Build and run the game"
	@echo "  make debug   - Rebuild with debug symbols (-g)"
	@echo "  make release - Rebuild with optimizations (-O2)"
	@echo "  make clean   - Remove object files and executable"
	@echo "  make help    - Show this help message"
