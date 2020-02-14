#!/bin/bash

cd "$(dirname "$0")"
gcc -O3 -o efelder *.c ../../lib/libSLK.a -lm -lSDL2 -ldl -lGL -lcjson -Wall
chmod +x efelder
./efelder
exit

/bin/bash
