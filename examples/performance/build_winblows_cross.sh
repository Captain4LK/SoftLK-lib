#!/bin/sh

cd "$(dirname "$0")"
x86_64-w64-mingw32-gcc -O3 -o performance main.c -L../../lib/windows_cross -lSLK -lmingw32 -lSDL2main -lSDL2 -mwindows -Wall
exit

