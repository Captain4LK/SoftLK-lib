#!/bin/sh

cd "$(dirname "$0")"
gcc -O3 -o brainfuck main.c -L../../lib/unix -lSLK -lm -lSDL2 -Wall
chmod +x brainfuck
./brainfuck
exit
