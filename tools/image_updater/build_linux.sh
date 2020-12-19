#!/bin/sh

gcc -O3 -o slk_img_updater main.c ../../external/UtilityLK/src/ULK_slk.c -lm -Wall
chmod +x slk_img_updater
./slk_img_updater
