## Resize example

This example demonstrates dynamic sized windows, layer scaling, moving and resizing. The image is drawn on a seperate layer, the window itself is resizeable with layer 1 being resized dynamicly. Hold the ctrl key and scroll  the mouse wheel to scale the layer, hold the mouse wheel and move the mouse to move it.

## How to compile

If you are on linux and have gcc installed you can just use the provided shell script to compile this example, otherwise you can compile it like this (again, clang should work too):


```
gcc -O3 -o resize main.c ../../lib/libSLK.a -lm -lSDL2 -lGL  -Wall #Compile to executable
chmod +x ./resize #Only needed on unix(-like) systems
```

## Code Example

This code example creates a resizable window with one dynamic layer.

```c
#include "../../include/SLK/SLK.h"

int main(int argc, char *argv[])
{
   //Create a resizable window
   //width,height,layer count,window title,fullscreen,pixel scale,dynamic
   SLK_setup(320,240,1,"Resizable window",0,3,1);

   //Create layer 0 as a rgb layer
   SLK_layer_create(0,SLK_LAYER_RGB);
   //Flag the layer as active (layers only get drawn if flagged as active)
   SLK_layer_activate(0,1);
   //Now flag the layer as dynamic. If the dynamic flag is set and SoftLK
   //has been started in dynamic mode, windows will be resizable and
   //the layer is resized accordingly to the window dimensions.
   SLK_layer_set_dynamic(0,1);

   while(SLK_core_running())
   {
      SLK_update();

      //Clear the layer so that it does not look weird when resized.
      SLK_layer_set_current(0);
      SLK_draw_rgb_set_clear_color(SLK_colore_create(255,128,0,255));
      SLK_draw_rgb_clear();

      SLK_render_update();
   }

   return 0;
}
```
