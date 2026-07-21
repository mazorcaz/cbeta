# cbeta makefile

# flags
CC := gcc
CFLAGS := -Wall -Wextra -Isrc

# files
SRCS := $(shell find src/ -name *.c)
HDRS := $(shell find src/ -name *.h)
OBJS := $(patsubst src/%.c,obj/%.o,$(SRCS))

.PHONY: all clean
	
all: bin/cbeta

clean:
	@echo "Cleaning"
	rm -rf obj/
	rm -rf bin/
	@echo

bin/cbeta: $(OBJS)
	@echo "Building $@"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@
	@echo

obj/%.o: src/%.c $(HDRS)
	@echo "Building $@"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo
