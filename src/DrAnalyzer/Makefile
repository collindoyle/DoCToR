#GNU Makefile

build ?= debug

PROJECT_ROOT := ../../

OUT := $(PROJECT_ROOT)build/$(build)

CFLAGS := -I../DrEntity/ -I../svmmulticlass

LIBS := $(OUT)/DrEntity.a 

ANALYZER_TARGET := $(OUT)/DrAnalyzer.a

ANALYZER_OBJS := DrAnalyzer.o

ANALYZER_SRC := DrAnalyzer.cpp

all: lib

lib: DrAnalyzer.o
	ar rc $(ANALYZER_TARGET) $(ANALYZER_OBJS) $(PROJECT_ROOT)/svmmulticlass

DrAnalyzer.o : DrAnalyzer.cpp DyAnalyzer.h
	$(CC) -c DrAnalyzer.cpp 