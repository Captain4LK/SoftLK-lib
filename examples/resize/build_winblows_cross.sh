#!/bin/sh

cd "$(dirname "$0")"
i686-w64-mingw32-gcc -O3 -o resize main.c -L../../lib/winblows_cross -lSLK -lmingw32 -lSDL2main -lSDL2  -lopengl32 -mwindows -Wall
exit
