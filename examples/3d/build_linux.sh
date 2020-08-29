#!/bin/sh

cd "$(dirname "$0")"
gcc -O3 -o 3d_demo main.c assets.c -L../../lib/unix/ -lSLK -lSLK3d_fixed -lm -lSDL2 -lGL -Wall
chmod +x 3d_demo
./3d_demo
exit

