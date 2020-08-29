#!/bin/bash

cd "$(dirname "$0")"
gcc -O3 -c ../src/*.c -lm -lSDL2 -lGL -Wall
ar cr libSLK.a *.o
exit

/bin/bash
