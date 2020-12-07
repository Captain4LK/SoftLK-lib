# Performance

This example messures the performance of the rgb and palettized software renderer included in  SoftLK. You can press r (rgb) and p(pal) to acess the two renderes and/or reset the timer. 

On the top left you can see to numbers. The number on the left displays the average render time since the last reset, the right number shows how many sprites are beeing drawn. You can add more sprites by pressing/holding the space bar. In the top right corner the currently used renderer is beeing displayed.

![screenshot](../../screenshots/performance.png)

## How to compile

If you are on linux and have gcc installed you can just use the provided shell script to compile this example (the build_linux.sh script also works for FreeBSD), otherwise you can compile it like this (again, clang should work too):


```
gcc -O3 -o performance main.c -L../../lib/unix/ -lSLK -lm -lSDL2 -lGL  -Wall #Compile to executable
chmod +x ./performance #Only needed on unix(-like) systems
```
