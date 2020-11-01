#!/bin/bash

cd "$(dirname "$0")"
i686-w64-mingw32-gcc -O3 -c ../../src/*.c ../../src/backend/backend_sdl2_gl.c -lSDL2 -lopengl32 -lmingw32 -Wall
ar cr libSLK.a *.o
exit
