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
//-------------------------------------

//Internal includes
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

int main(int argc, char *argv[])
{
   SLK_setup(320,240,2,"SoftLK template",0,SLK_WINDOW_MAX,1); 
   SLK_timer_set_fps(30);

   SLK_layer_create(0,SLK_LAYER_RGB);
   SLK_layer_activate(0,1);
   SLK_layer_set_current(0);
   SLK_layer_set_dynamic(0,0);
   SLK_draw_rgb_set_clear_color(SLK_color_create(128,128,128,255)); 
   SLK_draw_rgb_clear();
   SLK_draw_rgb_set_changed(1);
    
   SLK_layer_create(1,SLK_LAYER_RGB);
   SLK_layer_activate(1,1);
   SLK_layer_set_current(1);
   SLK_layer_set_dynamic(1,0);
   SLK_draw_rgb_set_clear_color(SLK_color_create(128,128,128,255)); 
   SLK_draw_rgb_clear();
   SLK_draw_rgb_set_changed(1);

   //Main loop.
   while(SLK_core_running())
   {
      SLK_update();

      if(SLK_key_pressed(SLK_KEY_D))
         puts("D pressed");
      SLK_layer_set_current(0);
      SLK_draw_rgb_set_clear_color(SLK_color_create(128,128,128,255)); 
      SLK_draw_rgb_clear();
      SLK_draw_rgb_set_changed(1);

      SLK_render_update();
   }

   return 0;
}
//-------------------------------------
