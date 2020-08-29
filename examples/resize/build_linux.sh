#!/bin/bash

cd "$(dirname "$0")"
gcc -O3 -o engine main.c -L../../lib/unix -lSLK -lm -lSDL2 -lGL -Wall
chmod +x engine
./engine
exit

/bin/bash
