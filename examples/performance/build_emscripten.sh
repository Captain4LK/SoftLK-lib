#!/bin/sh

emcc -O3 main.c ../../plattforms/emscripten/libSLK.a -lm -Wall -s USE_SDL=2 -s LLD_REPORT_UNDEFINED -s ASYNCIFY --preload-file assets --preload-file ../../data -o performance.html
