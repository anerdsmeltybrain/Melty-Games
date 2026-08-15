# Makefile for Raylib C project

# Compiler and Flags
CC      ?= gcc
CFLAGS  ?= -Wall -Wextra -std=c99 -O2
DFLAGS  ?= -Wall -Wextra -std=c99 -g -O0

# Target executable name
TARGET  := main

# Source and Object files
SRCS    := $(wildcard *.c)
OBJS    := $(SRCS:.c=.o)

# Raylib flags using pkg-config with fallbacks for Linux
INCLUDES := $(shell pkg-config --cflags raylib 2>/dev/null)
LIBS     := $(shell pkg-config --libs raylib 2>/dev/null || echo "-lraylib -lGL -lm -lpthread -ldl -lrt -lX11")

# Default rule
all: $(TARGET)

# Link executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LIBS)

# Compile C source files to object files
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Run the compiled program
run: $(TARGET)
	./$(TARGET)

# Debug build
debug: CFLAGS := $(DFLAGS)
debug: clean $(TARGET)

# Clean built files
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all run debug clean
