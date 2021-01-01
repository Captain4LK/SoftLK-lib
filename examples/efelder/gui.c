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

//This code is a big mess,
//just as always with gui code
//without the use of an actual gui framework.

//External includes
#include <time.h>
#include "../../include/SLK/SLK.h"
//-------------------------------------

//Internal includes
#include "gui.h"
#include "settings.h"
#include "calculate.h"
//-------------------------------------

//#defines
#define GUI_HELP_LINES 61
#define GUI_HELP_SCROLL 14
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
int mode;

static SLK_RGB_sprite *gui_top_right;
static SLK_RGB_sprite *gui_options;
static SLK_RGB_sprite *gui_help;
static SLK_RGB_sprite *gui_help_bar;
static SLK_RGB_sprite *gui_help_text;
static SLK_RGB_sprite *button_0_pressed;
static SLK_RGB_sprite *button_1_pressed;
static SLK_RGB_sprite *button_2_pressed;
static SLK_RGB_sprite *button_4_pressed; //Yes button_4
static int button_0_anim = 0;
static int button_1_anim = 0;
static int button_2_anim = 0;
static int button_4_anim = 0;
static int gui_mode;
static int can_x = 0;
static int can_y = 0;
static int scroll_y = 0;
static const int scroll_max = GUI_HELP_LINES*12-244;
static float can_scale = 1.0f;
static char text_help[GUI_HELP_LINES][36] = //only use 34 letters
{
   
   "This is free and unencumbered     ",
   "software released into the public ",
   "domain.                           ",
   "Anyone is free to copy, modify,   ",
   "publish, use, compile, sell, or   ",
   "distribute this software, either  ",
   "in source code form or as a       ",
   "compiled binary, for any purpose, ",
   "commercial or non-commercial, and ",
   "by any means.                     ",
   "In jurisdictions that recognize   ",
   "copyright laws, the author or     ",
   "authors of this software dedicate ",
   "any and all copyright interest in ",
   "the software to the public domain.",
   "We make this dedication for the   ",
   "benefit of the public at large and",
   "to the detriment of our heirs and ",
   "successors.                       ",
   "..................................",
   "..................................",
   "Using this program should be      ",
   "fairly easy, but I will try to    ",
   "explain everything here anyway.   ",
   "On the right side you should see 5",
   "buttons.                          ",
   "The first button, marked with a   ",
   "cogwheel acesses the options menu.",
   "Here you can change the UI scale, ",
   "the background color (NOTE: does  ",
   "apply to saved images) and the    ",
   "canvas dimensions (NOTE: only     ",
   "applies after recalculating).     ",
   "The second button, marked with an ",
   "i acesses this text.              ",
   "The third button, marked with a   ",
   "wrench will acess the editor, once",
   "it's implemented (hopefully soon).",
   "The fourth button, marked with a  ",
   "reload symbol recalculates        ",
   "everything, use this after        ",
   "changing the layout or the canvas ",
   "dimensions.                       ",
   "The fith button, marked with a    ",
   "floppy disk, saves the the current",
   "layout as a png image (both the   ",
   "electrical field and potential).  ",
   "                                  ",
   "If you don't have any other menu  ",
   "open, you can move arround the    ",
   "simulation. By holding down the   ",
   "mousewheel and moving the mouse   ",
   "you can move the canvas. By       ",
   "scrolling the mousewheel you can  ",
   "zoom in and out towards the mouse ",
   "pointer.                          ",
   "This text still is not finished   ",
   "and does not yet describe some    ",
   "functionalities like editing json ",
   "files, the only way to edit       ",
   "anything currently.               "
};
//-------------------------------------

//Function prototypes
//-------------------------------------

//Function implementations

void gui_init()
{
   //Load assets
   SLK_RGB_sprite *sheet = SLK_rgb_sprite_load("assets/gui.png");
   gui_top_right = SLK_rgb_sprite_create(44,323);
   gui_options = SLK_rgb_sprite_create(256,280);
   gui_help = SLK_rgb_sprite_create(288,280);
   gui_help_bar = SLK_rgb_sprite_create(10,12);
   gui_help_text = SLK_rgb_sprite_create(274,244);
   button_0_pressed = SLK_rgb_sprite_create(44,44);
   button_1_pressed = SLK_rgb_sprite_create(44,44);
   button_2_pressed = SLK_rgb_sprite_create(44,44);
   button_4_pressed = SLK_rgb_sprite_create(44,44);
   SLK_rgb_sprite_copy_partial(gui_top_right,sheet,0,0,229,64,44,323);
   SLK_rgb_sprite_copy_partial(gui_options,sheet,0,0,317,0,256,280);
   SLK_rgb_sprite_copy_partial(gui_help,sheet,0,0,610,0,288,280);
   SLK_rgb_sprite_copy_partial(gui_help_bar,sheet,0,0,273,76,10,12);
   SLK_rgb_sprite_copy_partial(button_0_pressed,sheet,0,0,273,88,44,44);
   SLK_rgb_sprite_copy_partial(button_1_pressed,sheet,0,0,273,136,44,44);
   SLK_rgb_sprite_copy_partial(button_2_pressed,sheet,0,0,273,184,44,44);
   SLK_rgb_sprite_copy_partial(button_4_pressed,sheet,0,0,273,280,44,44);
   SLK_rgb_sprite_destroy(sheet);

   //Setup layers
   int width_l,height_l;
   SLK_layer_get_size(5,&width_l,&height_l);
   SLK_layer_set_size(0,width_l/gui_scale,height_l/gui_scale);
   SLK_layer_set_scale(0,gui_scale);
   SLK_layer_set_pos(0,0,0);
   SLK_layer_set_size(1,canvas_width,canvas_height);
   SLK_layer_set_pos(1,can_x,can_y);
   SLK_layer_set_scale(1,can_scale);
   SLK_layer_set_size(2,canvas_width,canvas_height);
   SLK_layer_set_pos(2,can_x,can_y);
   SLK_layer_set_scale(2,can_scale);
   SLK_layer_set_size(3,canvas_width,canvas_height);
   SLK_layer_set_pos(3,can_x,can_y);
   SLK_layer_set_scale(3,can_scale);
   SLK_layer_set_current(0);
   SLK_draw_rgb_set_clear_color(SLK_color_create(0,0,0,0));
   SLK_draw_rgb_clear();
   SLK_draw_rgb_set_changed(1);

   //Draw text once
   int text_start = scroll_y/12;
   int text_start_pos = (text_start*12)-scroll_y;

   memset(gui_help_text->data,0,sizeof(SLK_Color)*gui_help_text->height*gui_help_text->width); 
   SLK_draw_rgb_set_target(gui_help_text);
   for(int i = text_start;i<text_start+22;i++)
   {
      if(i>=0&&i<GUI_HELP_LINES)
         SLK_draw_rgb_string(0,text_start_pos,1,text_help[i],SLK_color_create(222,238,214,255));

      text_start_pos+=12;
   }
   SLK_draw_rgb_set_target(NULL);
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
            else if(y>=72&&y<116)
            {
               button_1_anim = 10;
               gui_mode = 3;
            }
            else if(y>=120&&y<164)
            {
               button_2_anim = 10;
            }
            else if(y>=168&&y<212)
            {
               calculate();
            }
            else if(y>=216&&y<260)
            {
               button_4_anim = 10;

               SLK_Layer *layer0, *layer1, *layer2;
               char path[256] = "";
               layer0 = SLK_layer_get(1);
               layer1 = SLK_layer_get(2);
               layer2 = SLK_layer_get(3);
               SLK_RGB_sprite *sprite_to_save = SLK_rgb_sprite_create(layer0->type_1.target->width,layer0->type_1.target->height);
               SLK_draw_rgb_set_target(sprite_to_save);
               SLK_draw_rgb_set_clear_color(SLK_color_create(0,0,0,0));
               SLK_draw_rgb_clear();
               SLK_rgb_sprite_copy(sprite_to_save,layer1->type_1.target);
               SLK_draw_rgb_sprite(layer0->type_1.target,0,0);
               sprintf(path,"feldlinien_%ld.png",time(NULL));
               SLK_rgb_sprite_save(path,sprite_to_save);
               SLK_draw_rgb_clear();
               SLK_rgb_sprite_copy(sprite_to_save,layer2->type_1.target);
               SLK_draw_rgb_sprite(layer0->type_1.target,0,0);
               sprintf(path,"potential_%ld.png",time(NULL));
               SLK_rgb_sprite_save(path,sprite_to_save);
               sprintf(path,"key_%ld.png",time(NULL));
               SLK_rgb_sprite_save(path,key_potential);

               SLK_draw_rgb_set_target(NULL);
               SLK_rgb_sprite_destroy(sprite_to_save);
            }
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
            if(x>pos_x+88&&x<pos_x+101)
            {
               if(y>pos_y+36&&y<pos_y+49) //Button scale minus
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
               else if(y>pos_y+100&&y<pos_y+113) //Button red minus
               {
                  background_color.r-=5;
                  SLK_layer_set_current(5);
                  SLK_draw_rgb_set_clear_color(background_color);
                  SLK_draw_rgb_clear();
                  SLK_draw_rgb_set_changed(1);
               }
               else if(y>pos_y+132&&y<pos_y+145) //Button green minus
               {
                  background_color.g-=5;
                  SLK_layer_set_current(5);
                  SLK_draw_rgb_set_clear_color(background_color);
                  SLK_draw_rgb_clear();
                  SLK_draw_rgb_set_changed(1);
               }
               else if(y>pos_y+164&&y<pos_y+177) //Button blue minus
               {
                  background_color.b-=5;
                  SLK_layer_set_current(5);
                  SLK_draw_rgb_set_clear_color(background_color);
                  SLK_draw_rgb_clear();
                  SLK_draw_rgb_set_changed(1);
               }
               else if(y>pos_y+222&&y<pos_y+235) //Button width minus
               {
                  canvas_width-=100;
               }
               else if(y>pos_y+254&&y<pos_y+267) //Button height minus
               {
                  canvas_height-=100;
               }
            }
            else if(x>pos_x+212&&x<pos_x+225)
            {
               if(y>pos_y+36&&y<pos_y+49) //Button scale plus
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
               else if(y>pos_y+100&&y<pos_y+113) //Button red plus
               {
                  background_color.r+=5;
                  SLK_layer_set_current(5);
                  SLK_draw_rgb_set_clear_color(background_color);
                  SLK_draw_rgb_clear();
                  SLK_draw_rgb_set_changed(1);
               }
               else if(y>pos_y+132&&y<pos_y+145) //Button green plus
               {
                  background_color.g+=5;
                  SLK_layer_set_current(5);
                  SLK_draw_rgb_set_clear_color(background_color);
                  SLK_draw_rgb_clear();
                  SLK_draw_rgb_set_changed(1);
               }
               else if(y>pos_y+164&&y<pos_y+177) //Button blue plus
               {
                  background_color.b+=5;
                  SLK_layer_set_current(5);
                  SLK_draw_rgb_set_clear_color(background_color);
                  SLK_draw_rgb_clear();
                  SLK_draw_rgb_set_changed(1);
               }
               else if(y>pos_y+222&&y<pos_y+235) //Button width plus
               {
                  canvas_width+=100;
               }
               else if(y>pos_y+254&&y<pos_y+267) //Button height plus
               {
                  canvas_height+=100;
               }
            }
            
            //Button close
            if(x>pos_x+237&&x<pos_x+250&&y>pos_y+6&&y<pos_y+19)
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
            if(x>pos_x+269&&x<pos_x+282&&y>pos_y+6&&y<pos_y+19)
            {
               gui_mode = 0; 
               SLK_layer_set_current(0);
               SLK_draw_rgb_set_clear_color(SLK_color_create(0,0,0,0));
               SLK_draw_rgb_clear();
               SLK_draw_rgb_set_changed(1);
            }
         }
         
         int wheel = SLK_mouse_wheel_get_scroll();
         int redraw = 0;
         if(wheel>0)
         {
            //Up
            scroll_y-=GUI_HELP_SCROLL;
            redraw = 1;
         }
         else if(wheel<0)
         {
            //Down
            scroll_y+=GUI_HELP_SCROLL;
            redraw = 1;
         }

         if(scroll_y>scroll_max)
            scroll_y = scroll_max;

         if(scroll_y<0)
            scroll_y = 0;

         if(redraw)
         {
            int text_start = (scroll_y/12)-1;
            int text_start_pos = (text_start*12)-scroll_y;

            memset(gui_help_text->data,0,sizeof(SLK_Color)*gui_help_text->height*gui_help_text->width); 
            SLK_draw_rgb_set_target(gui_help_text);
            for(int i = text_start;i<text_start+22;i++)
            {
               if(i>=0&&i<GUI_HELP_LINES)
                  SLK_draw_rgb_string(0,text_start_pos,1,text_help[i],SLK_color_create(222,238,214,255));

               text_start_pos+=12;
            }
            SLK_draw_rgb_set_target(NULL);
         }
      }
      break;
   }

   if(SLK_key_pressed(SLK_KEY_M))
   {
      if(mode)
      {
         mode = 0;
         SLK_layer_activate(2,0);
         SLK_layer_activate(3,1);
      }
      else 
      {
         mode = 1;
         SLK_layer_activate(2,1);
         SLK_layer_activate(3,0);
      }
   }
}

void gui_draw()
{
   if(SLK_layer_get_resized(5))
   {
      int width_l,height_l;
      SLK_layer_get_size(5,&width_l,&height_l);
      SLK_layer_set_current(5);
      SLK_draw_rgb_set_clear_color(background_color);
      SLK_draw_rgb_clear();
      SLK_draw_rgb_set_changed(1);
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
   if(button_4_anim)
   {
      button_4_anim--;
      SLK_draw_rgb_sprite(button_4_pressed,width-44,216);
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
         SLK_draw_rgb_string(pos_x+124,pos_y+39,1,temp_str,SLK_color_create(203,219,252,255));
         sprintf(temp_str,"%03d",background_color.r);
         SLK_draw_rgb_string(pos_x+144,pos_y+103,1,temp_str,SLK_color_create(203,219,252,255));
         sprintf(temp_str,"%03d",background_color.g);
         SLK_draw_rgb_string(pos_x+144,pos_y+135,1,temp_str,SLK_color_create(203,219,252,255));
         sprintf(temp_str,"%03d",background_color.b);
         SLK_draw_rgb_string(pos_x+144,pos_y+167,1,temp_str,SLK_color_create(203,219,252,255));
         sprintf(temp_str,"%05d",canvas_width);
         SLK_draw_rgb_string(pos_x+136,pos_y+225,1,temp_str,SLK_color_create(203,219,252,255));
         sprintf(temp_str,"%05d",canvas_height);
         SLK_draw_rgb_string(pos_x+136,pos_y+257,1,temp_str,SLK_color_create(203,219,252,255));
      }
      break;
  case 3:
      {
         int pos_x = (width-gui_help->width)/2;
         int pos_y = (height-gui_help->height)/2;
         SLK_draw_rgb_sprite(gui_help,pos_x,pos_y);
         SLK_draw_rgb_sprite(gui_help_text,pos_x+7,pos_y+28);
      }
      break;
   }
}

#undef GUI_HELP_LINES
#undef GUI_HELP_SCROLL
