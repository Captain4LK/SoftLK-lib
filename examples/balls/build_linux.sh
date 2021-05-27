#!/bin/sh

cd "$(dirname "$0")"
gcc -O3 -o balls main.c -L../../lib/unix -lSLK -lm -lSDL2 -lGL -Wall -s
chmod +x balls
./balls
exit
