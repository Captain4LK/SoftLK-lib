#!/bin/sh

cd "$(dirname "$0")"
gcc -O3 -o efelder *.c ../../external/UtilityLK/src/ULK_vector.c -L../../lib/unix -lSLK -lm -lSDL2 -ldl -lGL -Wall
chmod +x efelder
./efelder
exit
