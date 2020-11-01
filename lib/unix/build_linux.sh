#!/bin/bash

cd "$(dirname "$0")"
gcc -O3 -c ../../src/*.c ../../src/backend/backend_sdl2_gl.c -lm -lSDL2 -lGL -Wall
ar cr libSLK.a *.o
exit
