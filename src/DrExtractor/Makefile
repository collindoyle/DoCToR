# GNU Makefile

build ?= debug

PROJECT_ROOT := ../../
OUT := $(PROJECT_ROOT)build/$(build)

EXTRACTOR_TARGET := $(OUT)/libdrextractor.a

CFLAGS := $(CFLAGS) -I$(PROJECT_ROOT)mupdf/include -I$(PROJECT_ROOT)src/DrEntity \
	-I$(PROJECT_ROOT)mupdf/thirdparty/freetype/include \
	-I$(PROJECT_ROOT)tinyxml2 \
	-m64

LIBS := $(PROJECT_ROOT)mupdf/build/$(build)/libmupdf.a 
	\
	$(OUT)/libdrentity.a $(OUT)/tinyxml2.o
	
EXTRACTOR_SRC := \
	DrPDFExtractor.cpp \
	DrTexGrouper.cpp \
	DrXMLInterpreter.cpp \
	EncodeConverter.cpp \
	EucencodeConverter.cpp \
	
EXTRACTOR_OBJ := \
	DrPDFExtractor.o \
	DrTexGrouper.o \
	DrXMLInterpreter.o \
	EncodeConverter.o \
	EucencodeConverter.o \
	
default: all

all: lib

lib: $(EXTRACTOR_OBJ)
	cd ../../tinyxml2; make all;
	ar rc $(EXTRACTOR_TARGET) $(EXTRACTOR_OBJ) $(LIBS)
	rm *.o

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@
	
clean:
	rm -f *.o
	


