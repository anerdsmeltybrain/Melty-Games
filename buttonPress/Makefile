# Compiler and flags
CC       ?= gcc
CFLAGS   ?= -Wall -Wextra -std=c99 -O2

# Raylib linker flags
RAYLIB_LIBS := $(shell pkg-config --libs raylib 2>/dev/null)
ifeq ($(RAYLIB_LIBS),)
    RAYLIB_LIBS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

RAYLIB_CFLAGS := $(shell pkg-config --cflags raylib 2>/dev/null)

CFLAGS  += $(RAYLIB_CFLAGS)
LDLIBS  += $(RAYLIB_LIBS)

# Project executable and source files
TARGET  := buttonPress
SRCS    := $(wildcard *.c)
OBJS    := $(SRCS:.c=.o)

.PHONY: all clean run debug re

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

debug: CFLAGS += -g -DDEBUG
debug: re

re: clean all

clean:
	rm -f $(OBJS) $(TARGET)
