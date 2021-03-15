#!/bin/sh

cd "$(dirname "$0")"
gcc -O3 -o gui main.c -L../../lib/unix -lSLK -lm -lSDL2 -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter
chmod +x gui
./gui
exit

