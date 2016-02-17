# 项目路径
CUR_DIR     = $(shell pwd)
SRC_DIR     = $(CUR_DIR)/src
LIB_DIR     = $(CUR_DIR)/lib
INCLUDE_DIR = $(CUR_DIR)/include
BUILD_DIR   = $(CUR_DIR)/build
OBJ_DIR     = $(BUILD_DIR)/obj
BIN_DIR     = $(BUILD_DIR)/bin
PROGRAM     = $(BIN_DIR)/step1

# 文件路径
EXTENSION   = cpp
FILES       = $(SRC_DIR)/*.$(EXTENSION) \
              $(SRC_DIR)/Control/*.$(EXTENSION) \
              $(SRC_DIR)/Planner/*.$(EXTENSION) \
              $(SRC_DIR)/Utils/*.$(EXTENSION)
SRCS        = $(wildcard $(FILES))
OBJS        = $(patsubst $(SRC_DIR)/%.$(EXTENSION), $(OBJ_DIR)/%.o, $(SRCS))
OBJS_NO_DIR = $(nodir $(OBJS))

# 编译器及选项
CC = g++
CFLAGS = -Wall -g
LDFLAGS = -lm

.PHONY: all

all:
	@echo $(OBJS_NO_DIR)
