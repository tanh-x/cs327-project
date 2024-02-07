SRC_DIR := src
INC_DIR := include

CFLAGS := -Wall -g -I$(INC_DIR)
LDFLAGS := -lm -lpng

SRCS := $(wildcard $(SRC_DIR)/**/*.c $(SRC_DIR)/*.c)
OBJS := $(SRCS:.c=.o)

# Target binary
TARGET := main

all: build $(TARGET) post_build

# Rule for building the application
$(TARGET): $(OBJS)
	gcc -o $@ $^ $(LDFLAGS)
	@chmod a+rx $(TARGET)

# Rule for compiling source files
%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

.PHONY: build post_build clean_objects clean

build:
	@echo -e "  ┌───────────"
	@echo -e "  │ Starting build..."
	@echo -e "  └──"

post_build:
	@echo -e "  ┌───────────"
	@echo -e "  │ Finished compile jobs, cleaning object files..."
	@echo -e "  └──"
	make clean_objects
	@echo -e "\n  ┌───────────"
	@echo -e "  │ Finished all Makefile rules. Binary should be \"$(TARGET)\""
	@echo -e "  │ "
	@echo -e "  │ See README file for optional flags."
	@echo -e "  └──\n"

clean_objects:
	find $(SRC_DIR) -type f -name '*.o' -exec rm {} +

clean: clean_objects
	@echo -e "  ┌───────────"
	@echo -e "  │ Cleaning object and binary files..."
	@echo -e "  └──"
	@rm -vf $(TARGET)
