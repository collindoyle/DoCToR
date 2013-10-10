# GNU Makefile

build ?= debug

OUT := build/$(build)


default: all

all: mupdflib svmlib Entity Extractor Analyzer

mupdflib:
	cd mupdf; make third libs;
	
svmlib:
	cd svmmulticlass; make ;
	
Entity:
	cd src/DrEntity; make ;
	
Extractor:
	cd src/DrExtractor; make ;
	
Analyzer:
	cd src/DrAnalyzer; make ;
	
clean:
	cd mupdf; make clean
	cd svmmulticlass; make clean
	cd DrEntity; make clean
	cd DrExtractor; make clean
	cd DrAnalyzer; make clean
	
.PHONY: all clean
