#GNU Makefile

build ?= debug

PROJECT_ROOT := ../../

SVM_ROOT := $(PROJECT_ROOT)/svmmulticlass

SVM_OBJS := $(SVM_ROOT)/svm_struct/svm_struct_learn.o $(SVM_ROOT)/svm_struct_learn_custom.o $(SVM_ROOT)/svm_struct_api.o $(SVM_ROOT)/svm_light/svm_hideo.o $(SVM_ROOT)/svm_light/svm_learn.o $(SVM_ROOT)/svm_light/svm_common.o $(SVM_ROOT)/svm_struct/svm_struct_common.o

OUT := $(PROJECT_ROOT)build/$(build)

CFLAGS := -I../DrEntity/ -I../../svmmulticlass -I$(PROJECT_ROOT)/mupdf/include

LIBS := $(OUT)/libdrentity.a 

ANALYZER_TARGET := $(OUT)/libdranalyzer.a

ANALYZER_OBJS := DrAnalyzer.o

ANALYZER_SRC := DrAnalyzer.cpp

all: lib

lib: DrAnalyzer.o
	ar rc $(ANALYZER_TARGET) $(ANALYZER_OBJS) $(SVM_OBJS)

DrAnalyzer.o : DrAnalyzer.cpp
	$(CC) -c $(CFLAGS) DrAnalyzer.cpp 
	
clean: 
	rm -f $(ANALYZER_TARGET);
	rm -f *.o