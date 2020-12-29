#!/bin/sh

emcc -O3 main.c assets.c ../../plattforms/emscripten/libSLK.a ../../plattforms/emscripten/libSLK3d_fixed.a -lm -Wall -s USE_SDL=2 -s ASYNCIFY -s LLD_REPORT_UNDEFINED --preload-file assets -o 3d.html
