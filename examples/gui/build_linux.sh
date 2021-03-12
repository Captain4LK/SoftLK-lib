#!/bin/sh

cd "$(dirname "$0")"
gcc -O3 -o gui main.c -L../../lib/unix -lSLK -lm -lSDL2 -Wall
chmod +x gui
./gui
exit

