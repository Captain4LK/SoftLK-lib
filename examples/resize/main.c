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
   SLK_setup(800,600,2,"SoftLK resize test",0,1,1);  //Create 2 layers, one for the background, the other one for the image.
   SLK_timer_set_fps(60);

   //Layer 1 is the background layer (layer 0 is the last to get drawn).
   SLK_layer_create(1,SLK_LAYER_RGB); //Use as a rgb layer, since we can controll when to send data to gpu.
   SLK_layer_activate(1,1); //This controlls wether layers actually get drawn.
   SLK_layer_set_current(1); //Sets wich layer is the current (default) draw target.
   SLK_layer_set_dynamic(1,1); //Make dynamic so it resizes with the window and we can keep the background consistent.
   SLK_draw_rgb_set_clear_color(SLK_color_create(128,128,128,255)); 
   SLK_draw_rgb_clear(); //Clear to color specified in above line.
   SLK_draw_rgb_set_changed(1); //Tells the backend that the texture has changed, on the sdl2_gl backend 
                                //this makes the backend sent the data to the gpu again.

   //These variables are used for keeping track of the layer pos and scale.
   int canvas_pos_x = 128;
   int canvas_pos_y = 128;
   float canvas_scale = 1.0f;
   SLK_layer_create(0,SLK_LAYER_PAL); //Second layer wil be a pal layer, since I want to 
                                      //demonstrate how good 256 colors can look.
   SLK_layer_activate(0,1);
   SLK_layer_set_current(0);
   SLK_layer_set_dynamic(0,0); //This layer won't be dynamic since its size is fixed.
   SLK_layer_set_pos(0,canvas_pos_x,canvas_pos_y); //Set the layer pos to the one specified in the variables.
   SLK_layer_set_scale(0,canvas_scale); //Set the layer scale too.
   SLK_draw_pal_set_clear_paxel(SLK_color_create_paxel(0,0));
   SLK_draw_pal_clear(); //Clear to paxel specified in above line.
   SLK_layer_set_palette(0,SLK_palette_load("assets/3-3-2.pal")); //Load the correct palete for the layer.
                                                                  //We will be using a rgb332 palette.

   SLK_Pal_sprite *logo = SLK_pal_sprite_load("assets/image.slk"); //Load a image from the assets folder.
   SLK_layer_set_size(0,logo->width,logo->height); //To be future proof we won't use a fixed size, but rather change the size to the actual size of the image.
   SLK_draw_pal_sprite(logo,0,0); //Draw the sprite to the current layer.
   SLK_pal_sprite_destroy(logo); //Destroy the image again since it's not used anymore.
   //Pal layer don't (yet) have a set_changed flag, so we don't need to set it here.

   //Main loop.
   while(SLK_core_running())
   {
      SLK_update(); //Handle events, such as input, window resizesing...

      //Only move the image if the mouse wheel is being held down.
      if(SLK_mouse_down(SLK_BUTTON_MIDDLE))
      {
         int x,y;
         SLK_mouse_get_relative_pos(&x,&y); //Get how much the mouse moved this frame...
         //... and move the layer accordingly.
         canvas_pos_x+=x; 
         canvas_pos_y+=y;
         SLK_layer_set_pos(0,canvas_pos_x,canvas_pos_y); //Update layer pos.
      }

      //We only want to zoom when the controll key is being held down.
      if(SLK_key_down(SLK_KEY_CTRL))
      {
         int wheel = SLK_mouse_wheel_get_scroll(); //Get how much the mouse wheel has moved (and in which direction).

         if(wheel<0)
         {
            //This code makes the scrolling go towards the mouse pointer.
            int mx,my;
            SLK_mouse_get_pos(&mx,&my);
            float x = ((float)mx-(float)canvas_pos_x)/canvas_scale;
            float y = ((float)my-(float)canvas_pos_y)/canvas_scale;
            float scale_change = -canvas_scale*0.15f;
            canvas_scale+=canvas_scale*0.15f;
            canvas_pos_x+=x*scale_change;
            canvas_pos_y+=y*scale_change;
            SLK_layer_set_pos(0,canvas_pos_x,canvas_pos_y);
            SLK_layer_set_scale(0,canvas_scale);
         }
         else if(wheel>0)
         {
            int mx,my;
            SLK_mouse_get_pos(&mx,&my);
            float x = ((float)mx-(float)canvas_pos_x)/canvas_scale;
            float y = ((float)my-(float)canvas_pos_y)/canvas_scale;
            float scale_change = canvas_scale*0.15f;
            canvas_scale-=canvas_scale*0.15f;
            canvas_pos_x+=x*scale_change;
            canvas_pos_y+=y*scale_change;
            SLK_layer_set_pos(0,canvas_pos_x,canvas_pos_y);
            SLK_layer_set_scale(0,canvas_scale);
         }
      }

      if(SLK_layer_get_resized(1)) //Only clear and sent data to gpu if it actually changed. This returns true between a resize and a render_update function call.
      {
         //Clear if changed, since new pixels will contain garbage data.
         SLK_layer_set_current(1);
         SLK_draw_rgb_set_changed(1);
         SLK_draw_rgb_set_clear_color(SLK_color_create(128,128,128,255));
         SLK_draw_rgb_clear();
      }

      SLK_render_update(); //Tell the backend to render everything to the screen.
   }

   return 0;
}
//-------------------------------------
