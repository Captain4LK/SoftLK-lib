#!/bin/sh

cd "$(dirname "$0")"
i686-w64-mingw32-gcc -O3 -o balls main.c -L../../lib/windows_cross -lSLK -Wall -s -luser32 -lgdi32 -lopengl32 -lgdiplus -mwindows
exit
