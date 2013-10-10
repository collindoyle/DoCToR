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
	EncodeConverter.cpp \
	EucencodeConverter.cpp \
	
EXTRACTOR_OBJ := \
	DrContentExtractor.o \
	DrPDFExtractor.o \
	DrTexGrouper.o \
	DrXMLInterpreter.o \
	EncodeConverter.o \
	EucencodeConverter.o \
	
default: all

all: lib

lib: $(EXTRACTOR_OBJ)
	ar rc $(EXTRACTOR_TARGET) $(EXTRACTOR_OBJ) $(LIBS)
	rm *.o
	
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
	
.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@
	


