SHELL=/bin/bash
BUILDDIR := build

CC     := clang
CFLAGS := -std=gnu23 -Wall
LDFLAGS := -l curl

HELPERS  := $(shell find src/lib -maxdepth 1 -name *.c -type f)
SOURCES  := $(shell find src -regextype egrep -regex 'src\/[0-9]{4}\/[0-9]{1,2}.c' -type f)
BINARIES := $(patsubst src/%.c, %, $(SOURCES))
TARGETS  := $(subst /,_,$(BINARIES))
RESULTS  := $(patsubst %_result, %, $(TARGETS))

ifeq ($(DEBUG), 1)
	CFLAGS += -O0 -g3

	ifeq ($(SANITIZE), 1)
		CFLAGS += -fsanitize=undefined,address
	endif
else
	CFLAGS += -O3 -march=native -flto=auto
endif

.PHONY: clean all

all: clean build run

clean:
	@rm -rf $(BUILDDIR)

build: $(BINARIES)

build_debug:
	@DEBUG=1 SANITIZE=1 $(MAKE) $(BINARIES)

memtest: clean build_debug
	for bin in $(sort $(TARGETS)); do \
		$(BUILDDIR)/$$bin > /dev/null 2>&1; \
		res=$$([[ $$? == 0 ]] && echo "passed" || echo "failed"); echo "$$bin: $$res"; \
	done

test: clean build_debug memtest
	for bin in $(sort $(TARGETS)); do \
		$(BUILDDIR)/$$bin > $(BUILDDIR)/$${bin}_result; \
	done

run: $(BINARIES)
	@for bin in $(sort $(TARGETS)); do \
		echo "=====$$bin=====" && $(BUILDDIR)/$$bin; \
	done
.SUFFIXES:
%: TARGET = $(patsubst %,src/%.c,$@)
%: BUILD  = $(subst /,_,$@)
%: INPUTS = $(TARGET) $(HELPERS)
%:
	@mkdir -p build

	@echo "Building $(TARGET): $(CC) $(CFLAGS) $(LDFLAGS) -o "build/$(BUILD)" $(INPUTS)"
	@$(CC) $(CFLAGS) $(LDFLAGS) -o "build/$(BUILD)" $(INPUTS)
