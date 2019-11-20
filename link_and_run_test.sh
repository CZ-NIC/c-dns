#!/usr/bin/env bash

# Fast, short script that links library into testing program
# This script is not intended for deploy, it's just for development

PWD=`pwd`

if [ ! `echo $LD_LIBRARY_PATH | grep $PWD` ]; then
    export LD_LIBRARY_PATH=$PWD/build:$LD_LIBRARY_PATH
fi

g++ -L$PWD/build -Isrc/ -std=c++17 -Wall main.cpp -lcdns \
    && valgrind --leak-check=full ./a.out
    

