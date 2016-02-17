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
OBJS        = $(patsubst %.$(EXTENSION), %.o, $(SRCS))

# 编译器及选项
CC = g++
CFLAGS = -Wall -g
LDFLAGS = -lm

.PHONY: all

all:
	@echo $(SRCS)
	@echo $(OBJS)