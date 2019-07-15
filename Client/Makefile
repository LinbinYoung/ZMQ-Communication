# Compiler
CC = gcc
C++ = g++
# Library
LIBS = -m64 -lzmq
# .h path
CFLAGS = -Wall -I. -I../common
CPPFLAGS = $(CFLAGS) -std=c++11
GOAL = server
# Directory
LOCALDIR = ./
COMMONDIR = ../common

$(LOCALDIR)/$(GOAL) : $(LOCALDIR)/main.o $(COMMONDIR)/ZMQNew.o $(COMMONDIR)/MSGIO.o $(COMMONDIR)/common.o
	g++ $^ $(LIBS) -o $@

$(LOCALDIR)/main.o : $(LOCALDIR)/main.cpp
	g++ $(CPPFLAGS) -c $^ -o $@

$(COMMONDIR)ZMQNew.o : $(COMMONDIR)/ZMQNew.c
	gcc $(CFLAGS) -c $^ -o $@

$(COMMONDIR)MSGIO.o : $(COMMONDIR)/MSGIO.cpp
	g++ $(CPPFLAGS) -c $^ -o $@

$(COMMONDIR)common.o : $(COMMONDIR)/common.cpp
	g++ $(CPPFLAGS) -c $^ -o $@

.PHONY: clean
clean:
	cd ../common && rm -rf *.o
	rm -rf *.o $(GOAL)