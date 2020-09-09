#!/bin/sh

cd "$(dirname "$0")"
gcc -O3 -o performance main.c -L../../lib/unix -lSLK -lm -lSDL2 -lGL -Wall
chmod +x performance
./performance
exit

