build ?= debug

OUT := build/$(build)
GEN := generated

CC = clang
LD = gcc


CFLAGS = $(SFLAGS) -O3 -fomit-frame-pointer -ffast-math -Wall -m64
CFLAGS += $(CFLAGS) -Iinclude -Iscripts -I$(GEN)
LDFLAGS = $(SFLAGS) -O3 -lm -Wall -m64
LIBS += $(XLIBS) -lm

all: mupdf_lib svm Entity #Extractor Analyzer
	mkdir  $(OUT);

.PHONY: all clean

clean: mupdf_clean svmmulticlass_clean

mupdf_clean:
	cd mupdf; make clean
	
svmmulticlass_clean:
	cd svmmulticlass; make clean

    
    
mupdf_lib: 
	cd mupdf; make third libs

	
svm:
	cd svmmulticlass; make all
	
Entity:

	ENTITY_SRC := \
		DrAttributeList.cpp \
		DrBox.cpp \
		DrChar.cpp \
		DrDocument.cpp \
		DrFontCache.cpp \
		DrFontDescriptor.cpp \
		DrLine.cpp \
		DrPage.cpp \
		DrPhrase.cpp \
		DrPoint.cpp \
		DrThumbnail.cpp \
		DrZone.cpp \
		
	ENTITY_LIB := $(OUT)/DrEntity.a
	CFLAGS += $(CFLAGS) -Imupdf
		