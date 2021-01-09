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
#include "../../include/SLK/SLK_gui.h"
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
   SLK_setup(800,500,2,"SoftLK template",0,SLK_WINDOW_MAX,1); 
   SLK_timer_set_fps(30);

   //Gui layer
   SLK_layer_create(0,SLK_LAYER_RGB);
   SLK_layer_activate(0,1);
   SLK_layer_set_current(0);
   SLK_layer_set_dynamic(0,1);
   SLK_draw_rgb_set_clear_color(SLK_color_create(28,28,28,255)); 
   SLK_draw_rgb_clear();
   SLK_draw_rgb_set_changed(1);

   //Create window
   SLK_gui_window *window = SLK_gui_window_create(10,10,320,240);
   SLK_gui_window_set_title(window,"Window test");
   SLK_gui_window_set_moveable(window,1);
   SLK_gui_set_colors(SLK_color_create(90,90,90,255),SLK_color_create(200,200,200,255),SLK_color_create(100,100,100,255),SLK_color_create(50,50,50,255),SLK_color_create(0,0,0,255));
   SLK_RGB_sprite *font = SLK_rgb_sprite_load("../../data/fonts/cursive2.png");
   SLK_gui_set_font(font);
   SLK_gui_set_font_color(SLK_color_create(0,0,0,255));
   SLK_gui_element *button_test = SLK_gui_button_create(6,32,64,14,"Test");
   SLK_gui_element *button_plus = SLK_gui_button_create(12,64,14,14,"+");
   SLK_gui_element *button_minus = SLK_gui_button_create(12,204,14,14,"-");
   SLK_gui_element *label_test = SLK_gui_label_create(70,32,248,200,"I'd just like to interject for a moment.  What you're\nreferring to as Linux,"\
"is in\nfact, GNU/Linux, or as I've\nrecently taken to calling it, GNU plus Linux."\
"Linux is not an operating system unto itself, but rather another free\ncomponent"\
" of a fully\nfunctioning GNU system made\nuseful by the GNU corelibs,\nshell"\
"utilities and vital\nsystem components comprising a full OS as defined by POSIX.");
   SLK_gui_window_add_element(window,button_test);
   SLK_gui_window_add_element(window,button_plus);
   SLK_gui_window_add_element(window,button_minus);
   SLK_gui_window_add_element(window,label_test);

   //Main loop.
   while(SLK_core_running())
   {
      SLK_update();
      int mx,my;
      SLK_mouse_get_layer_pos(0,&mx,&my);
      SLK_gui_window_update_input(window,SLK_mouse_get_state(SLK_BUTTON_LEFT),SLK_mouse_get_state(SLK_BUTTON_RIGHT),mx,my);

      SLK_layer_set_current(0);
      SLK_draw_rgb_set_changed(1);
      SLK_draw_rgb_clear();
      SLK_gui_window_draw(window);

      SLK_render_update();
   }

   return 0;
}
//-------------------------------------
