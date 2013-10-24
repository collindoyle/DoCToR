# GNU Makefile

build ?= debug

PROJECT_ROOT := ../../

OUT := $(PROJECT_ROOT)build/$(build)

CFLAGS := $(CFLAGS) -I$(PROJECT_ROOT)mupdf/include -m64 -std=c++0x

MUPDFLIBPATH := $(PROJECT_ROOT)/mupdf/build/$(build)

MUPDFLIBS := $(MUPDFLIBPATH)/libmupdf.a \
	$(MUPDFLIBPATH)/libjpeg.a \
	$(MUPDFLIBPATH)/libopenjpeg.a \
	$(MUPDFLIBPATH)/libz.a \
	$(MUPDFLIBPATH)/libfreetype.a

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
	DrZone.cpp \
	DrPoint.cpp \
	DrThumbnail.cpp \
	
ENTITY_OBJS := \
	DrAttributeList.o \
	DrBox.o \
	DrChar.o \
	DrDocument.o \
	DrFontCache.o \
	DrFontDescriptor.o \
	DrLine.o \
	DrPage.o \
	DrPhrase.o \
	DrZone.o \
	DrPoint.o \
	DrThumbnail.o \
	
ENTITY_TARGET := libdrentity.a





default: all

all: lib

lib: $(ENTITY_OBJS)
	ar rc $(OUT)/$(ENTITY_TARGET) $(ENTITY_OBJS) $(MUPDFLIBS)
	rm *.o

$(ENTITY_TARGET): $(ENTITY_OBJS)
	
.cpp.o:
	g++ $(CFLAGS) -c $<  -o $@
	
clean:
	rm -f *.o *.a; rm -f $(OUT)/$(ENTITY_TARGET);