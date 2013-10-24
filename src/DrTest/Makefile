#GNU Makefile

build ?= debug
PROJECT_ROOT := ../../
OUT := $(PROJECT_ROOT)build/$(build)
SRC_ROOT := $(PROJECT_ROOT)src

CFLAGS := $(CFLAGS) -I$(PROJECT_ROOT)mupdf/include \
	-I$(SRC_ROOT)/DrEntity/ -I$(SRC_ROOT)/DrExtractor -I$(SRC_ROOT)/DrAnalyzer \
	-m64
	
CXXFLAGS := $(CFLAGS)

TARGET := $(OUT)/DrTest
	
SRC := DrTest.cpp

OBJ := DrTest.o

default: all

all: $(TARGET) 

$(TARGET) : DrTest.o
	g++ DrTest.o -o $(TARGET)  -L $(PROJECT_ROOT)mupdf/build/debug  -lz -ljpeg -lopenjpeg -lfreetype -ljbig2dec -lmupdf -L $(OUT) -ldrentity -lm

DrTest.o : DrTest.cpp
	g++ $(CXXFLAGS) -c DrTest.cpp



clean:
	rm -rf *.o;
	rm -rf $(TARGET)