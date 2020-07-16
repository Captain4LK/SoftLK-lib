/* 
	Copyright (C) 2020 by Captain4LK (Lukas Holzbeierlein)

	This program is free software: you can redistribute it and/or modify
   	it under the terms of the GNU General Public License as published by
   	the Free Software Foundation, either version 3 of the License, or
   	(at your option) any later version.

   	This program is distributed in the hope that it will be useful,
   	but WITHOUT ANY WARRANTY; without even the implied warranty of
   	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   	GNU General Public License for more details.

   	You should have received a copy of the GNU General Public License
   	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

//This code is a big mess,
//just as always with gui code
//without the use of an actual gui framework.

//External includes
#include "../../include/SLK/SLK.h"
//-------------------------------------

//Internal includes
#include "gui.h"
#include "settings.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
static SLK_RGB_sprite *gui_top_right;
static SLK_RGB_sprite *gui_options;
static SLK_RGB_sprite *gui_help;
static SLK_RGB_sprite *button_0_pressed;
static SLK_RGB_sprite *button_1_pressed;
static SLK_RGB_sprite *button_2_pressed;
static int button_0_anim = 0;
static int button_1_anim = 0;
static int button_2_anim = 0;
static int gui_mode;
static int can_x = 0;
static int can_y = 0;
static float can_scale = 1.0f;
static char text_help[][32] = //only use 30 letters
{
   "",
};
//-------------------------------------

//Function prototypes
//-------------------------------------

//Function implementations

void gui_init()
{
   //Load assets
   SLK_RGB_sprite *sheet = SLK_rgb_sprite_load("assets/about.png");
   gui_top_right = SLK_rgb_sprite_create(44,164);
   gui_options = SLK_rgb_sprite_create(293,100);
   gui_help = SLK_rgb_sprite_create(293,324);
   button_0_pressed = SLK_rgb_sprite_create(44,44);
   button_1_pressed = SLK_rgb_sprite_create(44,44);
   button_2_pressed = SLK_rgb_sprite_create(44,44);
   SLK_rgb_sprite_copy_partial(gui_top_right,sheet,0,0,229,64,44,164);
   SLK_rgb_sprite_copy_partial(gui_options,sheet,0,0,317,0,293,100);
   SLK_rgb_sprite_copy_partial(gui_help,sheet,0,0,610,0,293,324);
   SLK_rgb_sprite_copy_partial(button_0_pressed,sheet,0,0,273,88,44,44);
   SLK_rgb_sprite_copy_partial(button_1_pressed,sheet,0,0,273,136,44,44);
   SLK_rgb_sprite_copy_partial(button_2_pressed,sheet,0,0,273,184,44,44);
   SLK_rgb_sprite_destroy(sheet);

   //Setup layers
   int width_l,height_l;
   SLK_layer_get_size(5,&width_l,&height_l);
   SLK_layer_set_size(0,width_l/gui_scale,height_l/gui_scale);
   SLK_layer_set_scale(0,gui_scale);
   SLK_layer_set_pos(0,0,0);
   SLK_layer_set_size(1,can_width,can_height);
   SLK_layer_set_pos(1,can_x,can_y);
   SLK_layer_set_scale(1,can_scale);
   SLK_layer_set_size(2,can_width,can_height);
   SLK_layer_set_pos(2,can_x,can_y);
   SLK_layer_set_scale(2,can_scale);
   SLK_layer_set_size(3,can_width,can_height);
   SLK_layer_set_pos(3,can_x,can_y);
   SLK_layer_set_scale(3,can_scale);
   SLK_layer_set_current(0);
   SLK_draw_rgb_set_clear_color(SLK_color_create(0,0,0,0));
   SLK_draw_rgb_clear();
   SLK_draw_rgb_set_changed(1);
}

void gui_update()
{
   int width,height;
   SLK_layer_get_size(0,&width,&height);

   switch(gui_mode)
   {
   case 0: //electrical fields
   case 1: //electric potential
      if(SLK_mouse_down(SLK_BUTTON_MIDDLE))
      {
         int x,y;
         SLK_mouse_get_relative_pos(&x,&y);
         can_x+=x;
         can_y+=y;
         SLK_layer_set_pos(1,can_x,can_y);
         SLK_layer_set_pos(2,can_x,can_y);
         SLK_layer_set_pos(3,can_x,can_y);
      }

      if(SLK_mouse_pressed(SLK_BUTTON_LEFT))
      {
         int x,y;
         SLK_mouse_get_layer_pos(0,&x,&y);
         if(x>=width-44&&x<width) 
         {
            if(y>=24&&y<68)
            {
               button_0_anim = 10;
               gui_mode = 2;
            }
            if(y>=72&&y<116)
            {
               button_1_anim = 10;
               gui_mode = 3;
            }
            if(y>=120&&y<164)
               button_2_anim = 10;
         }
      }

      int wheel = SLK_mouse_wheel_get_scroll();

      if(wheel<0)
      {
         int mx,my;
         SLK_mouse_get_pos(&mx,&my);
         float x = ((float)mx-(float)can_x)/can_scale;
         float y = ((float)my-(float)can_y)/can_scale;
         float scale_change = -can_scale*0.15f;
         can_scale+=can_scale*0.15f;
         can_x+=x*scale_change;
         can_y+=y*scale_change;
         SLK_layer_set_pos(1,can_x,can_y);
         SLK_layer_set_pos(2,can_x,can_y);
         SLK_layer_set_pos(3,can_x,can_y);

         SLK_layer_set_scale(1,can_scale);
         SLK_layer_set_scale(2,can_scale);
         SLK_layer_set_scale(3,can_scale);
      }
      else if(wheel>0)
      {
         int mx,my;
         SLK_mouse_get_pos(&mx,&my);
         float x = ((float)mx-(float)can_x)/can_scale;
         float y = ((float)my-(float)can_y)/can_scale;
         float scale_change = can_scale*0.15f;
         can_scale-=can_scale*0.15f;
         can_x+=x*scale_change;
         can_y+=y*scale_change;
         SLK_layer_set_pos(1,can_x,can_y);
         SLK_layer_set_pos(2,can_x,can_y);
         SLK_layer_set_pos(3,can_x,can_y);

         SLK_layer_set_scale(1,can_scale);
         SLK_layer_set_scale(2,can_scale);
         SLK_layer_set_scale(3,can_scale);
      }
     
      break;
   case 2: //Options
      {
         int pos_x = (width-gui_options->width)/2;
         int pos_y = (height-gui_options->height)/2;
         if(SLK_mouse_pressed(SLK_BUTTON_LEFT))
         {
            int x,y;
            SLK_mouse_get_layer_pos(0,&x,&y);
            if(y>pos_y+78&&y<pos_y+95)
            {
               if(x>pos_x+86&&x<pos_x+103&&gui_scale>0.1f)
               {
                  gui_scale-=0.1f;
                  int width_l,height_l;
                  SLK_layer_get_size(5,&width_l,&height_l);
                  SLK_layer_set_size(0,width_l/gui_scale,height_l/gui_scale);
                  SLK_layer_set_scale(0,gui_scale);
                  SLK_layer_set_pos(0,0,0);
                  SLK_layer_set_current(0);
                  SLK_draw_rgb_set_clear_color(SLK_color_create(0,0,0,0));
                  SLK_draw_rgb_clear();
                  SLK_draw_rgb_set_changed(1);
               }
               else if(x>pos_x+210&&x<+pos_x+227&&gui_scale<10.0f)
               {
                  gui_scale+=0.1f;
                  int width_l,height_l;
                  SLK_layer_get_size(5,&width_l,&height_l);
                  SLK_layer_set_size(0,width_l/gui_scale,height_l/gui_scale);
                  SLK_layer_set_scale(0,gui_scale);
                  SLK_layer_set_pos(0,0,0);
                  SLK_layer_set_current(0);
                  SLK_draw_rgb_set_clear_color(SLK_color_create(0,0,0,0));
                  SLK_draw_rgb_clear();
                  SLK_draw_rgb_set_changed(1);
               }
            }
            if(x>pos_x+245&&x<pos_x+286&&y>pos_y+8&&y<pos_y+56)
            {
               gui_mode = 0; 
               SLK_layer_set_current(0);
               SLK_draw_rgb_set_clear_color(SLK_color_create(0,0,0,0));
               SLK_draw_rgb_clear();
               SLK_draw_rgb_set_changed(1);
            }
         }
      }
      break;
   case 3: //help
      {
         int pos_x = (width-gui_help->width)/2;
         int pos_y = (height-gui_help->height)/2;
         if(SLK_mouse_pressed(SLK_BUTTON_LEFT))
         {
            int x,y;
            SLK_mouse_get_layer_pos(0,&x,&y);
            if(x>pos_x+245&&x<pos_x+286&&y>pos_y+8&&y<pos_y+56)
            {
               gui_mode = 0; 
               SLK_layer_set_current(0);
               SLK_draw_rgb_set_clear_color(SLK_color_create(0,0,0,0));
               SLK_draw_rgb_clear();
               SLK_draw_rgb_set_changed(1);
            }
         }

      }
      break;
   }
}

void gui_draw()
{
   if(SLK_layer_get_resized(5))
   {
      int width_l,height_l;
      SLK_layer_get_size(5,&width_l,&height_l);
      SLK_layer_set_size(0,width_l/gui_scale,height_l/gui_scale);
      SLK_layer_set_scale(0,gui_scale);
      SLK_layer_set_pos(0,0,0);
      SLK_layer_set_current(0);
      SLK_draw_rgb_set_clear_color(SLK_color_create(0,0,0,0));
      SLK_draw_rgb_clear();
      SLK_draw_rgb_set_changed(1);
   }
   SLK_layer_set_current(0);
   SLK_draw_rgb_set_changed(1);
   int width,height;
   SLK_layer_get_size(0,&width,&height);

   //Gets always drawn
   SLK_draw_rgb_sprite(gui_top_right,width-44,0); 
   if(button_0_anim)
   {
      button_0_anim--;
      SLK_draw_rgb_sprite(button_0_pressed,width-44,24);
   }
   if(button_1_anim)
   {
      button_1_anim--;
      SLK_draw_rgb_sprite(button_1_pressed,width-44,72);
   }
   if(button_2_anim)
   {
      button_2_anim--;
      SLK_draw_rgb_sprite(button_2_pressed,width-44,120);
   }

   //Modes:
   //0,1: unused/default
   //2: settings
   //3: help
   switch(gui_mode)
   {
   case 2:
      {
         int pos_x = (width-gui_options->width)/2;
         int pos_y = (height-gui_options->height)/2;
         SLK_draw_rgb_sprite(gui_options,pos_x,pos_y);
         char temp_str[256] = "";
         sprintf(temp_str,"%03f",gui_scale);
         SLK_draw_rgb_string(pos_x+128,pos_y+83,1,temp_str,SLK_color_create(203,219,252,255));
      }
      break;
  case 3:
      {
         int pos_x = (width-gui_help->width)/2;
         int pos_y = (height-gui_help->height)/2;
         SLK_draw_rgb_sprite(gui_help,pos_x,pos_y);
      }
      break;
   }
}
