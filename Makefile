# 项目路径
CUR_DIR     = $(shell pwd)
SRC_DIR     = $(CUR_DIR)/src
LIB_DIR     = $(CUR_DIR)/lib
INCLUDE_DIR = $(SRC_DIR)/include
SOURCE_DIR  = $(SRC_DIR)/source
BUILD_DIR   = $(CUR_DIR)/build
OBJ_DIR     = $(BUILD_DIR)/obj
BIN_DIR     = $(BUILD_DIR)/bin
PROGRAM     = step1

# 文件路径
EXTENSION   = cpp
SRCS        = $(wildcard $(SOURCE_DIR)/*.$(EXTENSION))
OBJS        = $(patsubst %.$(EXTENSION), $(OBJ_DIR)/%.o, $(notdir $(SRCS)))

# 编译器及选项
CC = g++
CFLAGS = -Wall -g
LDFLAGS = -lm

.PHONY: clean rebuild help

all: $(OBJS) $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/$(PROGRAM) $(OBJS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SOURCE_DIR)/%.$(EXTENSION) $(OBJ_DIR)
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

$(BIN_DIR): $(BUILD_DIR)
	mkdir $(BIN_DIR)

$(OBJ_DIR): $(BUILD_DIR)
	mkdir $(OBJ_DIR)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean all

help:
	@echo "make         -------- make target by modify time"
	@echo "make clean   -------- clean target"
	@echo "make rebuild -------- rebuild the target"