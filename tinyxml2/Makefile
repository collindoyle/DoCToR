# GNU Makefile

build ?= debug

PROJECT_ROOT := ../
OUT := $(PROJECT_ROOT)build/$(build)

TARGET := $(OUT)/tinyxml2.o

default: all

all: tinyxml2o

tinyxml2o: tinyxml2.o

tinyxml2.o: tinyxml2.cpp
	$(CC) -c tinyxml2.cpp -o tinyxml2.o
	cp tinyxml2.o $(OUT); \
	
	

clean:
	rm -f $(TARGET); \
	rm -f *.o;
