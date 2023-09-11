#!/bin/bash

CC=gcc
HEADER=spxe/spxe.h
SRC=spxepy.c
LIB=spxepy.so
STD=-std=c99
OPT=-O2
SPXOBJ=spxe.o
PYOBJ=spxepy.o

PYPATH=/usr/local/Cellar//python@3.10/3.10.13/Frameworks/Python.framework/Versions/3.10
LIBS=-L$(PYPATH)/lib/ -lpython3.10 -lglfw -framework OpenGL -fPIC -shared
INC=-Ispxe -I$(PYPATH)/include/python3.10/
WFLAGS=-Wall -Wextra -pedantic -Wno-unused-parameter

CFLAGS=$(STD) $(OPT) $(WFLAGS)

$(LIB): $(SPXOBJ) $(PYOBJ)
	$(CC) $(OPT) $(LIBS) $^ -o $@

$(SPXOBJ): $(HEADER)
	$(CC) $(CFLAGS) -x c -DSPXE_APPLICATION -c $^ -o $@

$(PYOBJ): $(SRC)
	$(CC) $(CFLAGS) $(INC) -c $^ -o $@

.PHONY: clean

clean:
	$(RM) $(LIB)
	$(RM) $(SPXOBJ)
	$(RM) $(PYOBJ)

