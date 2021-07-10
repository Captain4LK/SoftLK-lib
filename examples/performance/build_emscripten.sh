#!/bin/sh

emcc -O3 main.c ../../lib/emscripten/libSLK.a -lm -Wall -s USE_SDL=2 -s ASYNCIFY -s LLD_REPORT_UNDEFINED --preload-file assets --preload-file ../../data -o performance.html
