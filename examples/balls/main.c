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
#include <stdlib.h>
#include <time.h>
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
   SLK_setup(320,240,2,"SoftLK balls",0,SLK_WINDOW_MAX,1); 
   SLK_timer_set_fps(30);

   SLK_layer_create(0,SLK_LAYER_PAL);
   SLK_layer_activate(0,1);
   SLK_layer_set_current(0);
   SLK_layer_set_dynamic(0,0);
   SLK_layer_set_palette(0,SLK_palette_load("king-16.pal"));
   SLK_draw_pal_set_clear_index(0);
   SLK_draw_pal_clear();

   //Generate balls
   srand(time(NULL));
   struct
   {
      int r;
      int index;
      int x;
      int y;
      int dx;
      int dy;
   }balls[64];
   for(int i = 0;i<64;i++)
   {
      balls[i].x = 10+rand()%300;
      balls[i].y = 10+rand()%220;
      balls[i].r = 8+rand()%8;
      balls[i].index = 1+rand()%7;
      balls[i].dx = 1+rand()%4;
      balls[i].dy = 1+rand()%4;
   }

   //Main loop.
   while(SLK_core_running())
   {
      SLK_update();
      SLK_layer_set_current(0);
      SLK_draw_pal_clear();

      for(int i = 0;i<64;i++)
      {
         //Ball movement
         balls[i].x+=balls[i].dx;
         balls[i].y+=balls[i].dy;
         if(balls[i].x+balls[i].r>=320) { balls[i].x = 320-balls[i].r; balls[i].dx = 1+rand()%4; balls[i].dx*=-1; }
         if(balls[i].x<=balls[i].r) { balls[i].x = balls[i].r; balls[i].dx = 1+rand()%4;}
         if(balls[i].y+balls[i].r>=240) { balls[i].y = 240-balls[i].r; balls[i].dy = 1+rand()%4; balls[i].dy*=-1; }
         if(balls[i].y<=balls[i].r) { balls[i].y = balls[i].r; balls[i].dy = 1+rand()%4; }

         //Draw balls
         SLK_draw_pal_fill_circle(balls[i].x,balls[i].y,balls[i].r,balls[i].index);
         SLK_draw_pal_fill_circle(balls[i].x+balls[i].r/3,balls[i].y+balls[i].r/3,balls[i].r/2,balls[i].index+8);
      }

      SLK_render_update();
   }

   return 0;
}
//-------------------------------------
