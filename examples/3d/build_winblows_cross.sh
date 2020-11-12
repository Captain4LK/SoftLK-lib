#!/bin/sh

cd "$(dirname "$0")"
i686-w64-mingw32-gcc -O3 -o 3d_demo main.c assets.c -L../../lib/windows_cross -lSLK -lSLK3d_fixed -lmingw32 -lSDL2main -lSDL2  -lopengl32 -mwindows -Wall
exit
