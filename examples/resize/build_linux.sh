#!/bin/sh

cd "$(dirname "$0")"
gcc -O3 -o resize main.c -L../../lib/unix -lSLK -lm -lSDL2 -lGL -Wall
chmod +x resize
./resize
exit
