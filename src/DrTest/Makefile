#GNU Makefile

build ?= debug
PROJECT_ROOT := ../../
OUT := $(PROJECT_ROOT)build/$(build)
SRC_ROOT := $(PROJECT_ROOT)src

CFLAGS := $(CFLAGS) -I$(PROJECT_ROOT)mupdf/include \
	-I$(SRC_ROOT)/DrEntity/ -I$(SRC_ROOT)/DrExtractor -I$(SRC_ROOT)/DrAnalyzer \
	-m64
	
TARGET := $(OUT)/DrTest
	
SRC := DrTest.cpp

OBJ := DrTest.o

default: all

all: $(TARGET) 

$(TARGET) : DrTest.o DrTest.cpp
	$(CC) -c -o DrTest DrTest.cpp




clean: