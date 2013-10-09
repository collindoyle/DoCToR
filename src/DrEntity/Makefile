# GNU Makefile
build ?= debug

CC := gcc
LD := gcc

PROJECT_ROOT := ../../
OUT := $(PROJECT_ROOT)build/$(build)



CFLAGS += -I$(PROJECT_ROOT)mupdf/include/mupdf/ -I$(PROJECT_ROOT)mupdf/include -m64
LIBS += -l$(PROJECT_ROOT)mupdf/build/$(build)/libmupdf.a -lm 

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
		DrPoint.o \
		DrThumbnail.o \
		DrZone.o \
		
ENTITY_LIB := $(OUT)/DrEntity.a


default: all
all: lib

lib: $(ENTITY_LIB) 
$(ENTITY_LIB): $(ENTITY_OBJS)
	ar rc $(ENTITY_LIB) $(ENTITY_OBJS) 
	ranlib $(ENTITY_LIB)
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< 
	
$(ENTITY_OBJS): $(PROJECT_ROOT)mupdf/include/mupdf/fitz.h

clean:
	rm -f *.o *.a
