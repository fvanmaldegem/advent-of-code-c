SHELL=/bin/bash

CC     := clang
CFLAGS := -std=gnu23 -Wall
LDFLAGS := -l curl

HELPERS  := $(shell find src/lib -maxdepth 1 -name *.c -type f)
TARGETS  := $(shell find src -name day*.c -type f)
BINARIES := $(patsubst src/%.c, %, $(TARGETS))

ifeq ($(DEBUG), 1)
	CFLAGS += -O0 -g3

	ifeq ($(SANITIZE), 1)
		CFLAGS += -fsanitize=undefined,address
	endif
else
	CFLAGS += -O3 -march=native -flto=auto
endif

.PHONY: all clean

all: $(BINARIES)

clean:
	rm -rf $(BIN)

.SUFFIXES:
%: TARGET = $(patsubst %,src/%.c,$@)
%: BUILD  = $(subst /,_,$@)
%: INPUTS = $(TARGET) $(HELPERS)
%:
	@rm -rf build
	@mkdir build

	@echo "Building $(TARGET):"
	$(CC) $(CFLAGS) $(LDFLAGS) -o "build/$(BUILD)" $(INPUTS) 
	@echo "Build $(BUILD)"