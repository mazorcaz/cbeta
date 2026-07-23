# cbeta makefile

# flags
CC := gcc
CFLAGS := -Wall -Wextra -Iinclude $(shell sdl2-config --cflags)
LDLIBS := -lSDL2 -lSDL2_image -lGL -lm

# files
SRCS := $(shell find src/ -name "*.c")
HDRS := $(shell find include/ -name "*.h")
OBJS := $(patsubst src/%.c,obj/%.o,$(SRCS))

.PHONY: all clean
	
all: bin/cbeta

clean:
	@echo "Cleaning"
	rm -rf obj/
	rm -rf bin/
	@echo

bin/resources/%: resources/%
	@echo "Building resource $@"
	@mkdir -p $(dir $@)
	cp -r $< $@
	@echo

bin/cbeta: $(OBJS) $(patsubst %,bin/%,$(shell find resources/ -mindepth 1))
	@echo "Building $@"
	@mkdir -p $(dir $@)
	$(CC) $(OBJS) -o $@ $(LDLIBS)
	@echo

obj/%.o: src/%.c $(HDRS)
	@echo "Building $@"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo
