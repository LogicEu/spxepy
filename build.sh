#!/bin/bash

cc=gcc
header=spxe/spxe.h
src=spxepy.c
lib=spxepy.so
std=-std=c99
opt=-O2
spxobj=spxe.o
pyobj=spxepy.o

pypath=/usr/local/Cellar//python@3.10/3.10.13/Frameworks/Python.framework/Versions/3.10

libs=(
    -L$pypath/lib/
    -lpython3.10
    -lglfw
    -framework OpenGL
    -fPIC
    -shared
)

inc=(
    -Ispxe
    -I$pypath/include/python3.10/
)

wflags=(
    -Wall
    -Wextra
    -pedantic
    -Wno-unused-parameter
)

cmd() {
    echo "$@" && $@
}

compile() {
    cmd $cc $std $opt ${wflags[*]} -x c -DSPXE_APPLICATION -c $header -o $spxobj
    cmd $cc $std $opt ${wflags[*]} ${inc[*]} -c $src -o $pyobj
    cmd $cc $opt ${libs[*]} $spxobj $pyobj -o $lib
}

cleanf() {
    [ -f $1 ] && cmd rm -f $1
}

clean() {
    cleanf $lib
    cleanf $spxobj
    cleanf $pyobj
}

(( $# < 1 )) && compile && exit

case "$1" in
    "clean")
        clean;;
esac

