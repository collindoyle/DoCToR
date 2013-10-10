# GNU Makefile

build ?= debug

PROJECT_ROOT := ../../
OUT := $(PROJECT_ROOT)build/$(build)
EXTRACTOR_TARGET := $(OUT)/DrExtractor.a

CFLAGS := $(CFLAGS) -I$(PROJECT_ROOT)mupdf/include -m64

LIBS := $(PROJECT_ROOT)mupdf/build/$(build)/libmupdf.a \
	$(OUT)/DrEntity.a
	
EXTRACTOR_SRC := \
	DrContentExtractor.c \
	DrPDFExtractor.cpp \
	DrTexGrouper.cpp \
	DrXMLInterpreter.cpp \
	

