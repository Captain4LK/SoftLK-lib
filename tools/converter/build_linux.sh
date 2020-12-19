#!/bin/sh

gcc -O3 -o slk_converter main.c ../../external/UtilityLK/src/ULK_slk.c -lm -Wall
chmod +x slk_converter
./slk_converter
