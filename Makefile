# Well, if this aint the ugliest most beautifull bash riddled Makefile you have even seen, i dunno anymore. 
SHELL=/bin/bash
BUILDDIR := build

CC     := clang
CFLAGS := -std=gnu23 -Wall
LDFLAGS := -l curl

HELPERS  := $(shell find src/lib -maxdepth 1 -name *.c -type f)
SOURCES  := $(shell find src -regextype egrep -regex 'src\/[0-9]{4}\/[0-9]{1,2}.c' -type f)
BINARIES := $(patsubst src/%.c, %, $(SOURCES))
TARGETS  := $(subst /,_,$(BINARIES))

ifeq ($(DEBUG), 1)
	CFLAGS += -O0 -g3

	ifeq ($(SANITIZE), 1)
		CFLAGS += -fsanitize=undefined,address
	endif
else
	CFLAGS += -O3 -march=native -flto=auto
endif

.PHONY: clean all build build_debug sanitation_test test run

all: clean build run

clean:
	@rm -rf $(BUILDDIR)

build: $(BINARIES)

build_debug:
	@DEBUG=1 SANITIZE=1 $(MAKE) $(BINARIES)

sanitization_test: clean build_debug
	@echo "====Sanitization Test===="
	@for bin in $(sort $(TARGETS)); do \
		$(BUILDDIR)/$$bin > /dev/null 2>&1; \
		res=$$([[ $$? == 0 ]] && echo "passed" || echo "failed"); echo "$$bin: $$res"; \
	done

unit_test: clean build_debug sanitization_test
	@for bin in $(sort $(TARGETS)); do \
		$(BUILDDIR)/$${bin} > $(BUILDDIR)/$${bin}_result; \
	done

	@echo "====Unit Test===="
	@for bin in $(sort $(TARGETS)); do \
		expected="$$(echo $$bin | tr '_' '/')/expected.txt"; \
		diff $(BUILDDIR)/$${bin}_result test/$${expected}; \
		res=$$([[ $$? == 0 ]] && echo "passed" || echo "failed"); echo "$$bin: $$res"; \
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
