#!/bin/sh

cd "$(dirname "$0")"
#x86_64-w64-mingw32-gcc -O3 -o resize main.c -L../../lib/windows_cross -lSLK -lmingw32 -lSDL2main -lSDL2 -mwindows -Wall
i686-w64-mingw32-gcc -O3 -o resize main.c -L../../lib/windows_cross -lSLK -Wall -s -luser32 -lgdi32 -lopengl32 -lgdiplus -mwindows
exit
