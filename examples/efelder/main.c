/*
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>
*/

//External includes
#include "../../include/SLK/SLK.h"
#include "../../external/UtilityLK/include/ULK_vector.h"
//-------------------------------------

//Internal includes
#include "gui.h"
#include "settings.h"
#include "calculate.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
//-------------------------------------

//Function prototypes
//-------------------------------------

//Function implementations

int main(int argc, char **argv)
{
   if(argc<2)
      return -1;

   settings_init_default();
   shapes_load_file(argv[1]);

   SLK_setup(800,600,6,"Efelder",0,1,1);
   SLK_core_set_visible(0);
   SLK_timer_set_fps(30);

   SLK_layer_create(0,SLK_LAYER_RGB); //Layer for GUI
   SLK_layer_create(1,SLK_LAYER_RGB); //Layer for shapes
   SLK_layer_create(2,SLK_LAYER_RGB); //Layer for drawing electric field
   SLK_layer_create(3,SLK_LAYER_RGB); //Layer for drawing electric potential
   SLK_layer_create(4,SLK_LAYER_RGB); //Layer for editing layout
   SLK_layer_create(5,SLK_LAYER_RGB); //Layer for background

   SLK_layer_activate(0,1);
   SLK_layer_set_dynamic(0,0);
   SLK_layer_activate(1,1);
   SLK_layer_set_dynamic(1,0);
   SLK_layer_activate(2,0);
   SLK_layer_set_dynamic(2,0);
   SLK_layer_activate(3,1);
   SLK_layer_set_dynamic(3,0);
   SLK_layer_activate(4,0);
   SLK_layer_set_dynamic(4,0);
   SLK_layer_activate(5,1);
   SLK_layer_set_dynamic(5,1);

   gui_init();

   calculate();

   while(SLK_core_running())
   {
      SLK_update();
      gui_update();

      gui_draw();
      SLK_render_update();
   }

   return 0;
}
//-------------------------------------
