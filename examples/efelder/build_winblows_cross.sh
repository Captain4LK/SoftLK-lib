#!/bin/sh

cd "$(dirname "$0")"
i686-w64-mingw32-gcc -O3 -o efelder *.c ../../external/UtilityLK/src/ULK_vector.c ../../external/UtilityLK/src/ULK_json.c -L../../lib/windows_cross -lSLK -lmingw32 -lSDL2main -lSDL2 -lopengl32 -mwindows -Wall
exit
