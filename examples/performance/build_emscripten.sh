#!/bin/sh

emcc -O2 main.c ../../plattforms/emscripten/libSLK.a -lm -Wall -s USE_SDL=2 -s USE_LIBPNG=1 -s LLD_REPORT_UNDEFINED --preload-file assets --preload-file ../../data -o performance.html
