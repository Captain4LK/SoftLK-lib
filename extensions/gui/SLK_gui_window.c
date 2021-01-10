/*
SLK_gui - gui extension for SoftLK-lib

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

This work's goal is to never be encumbered by any exclusive intellectual
property rights. The work is therefore provided under CC0 1.0 + additional
WAIVER OF ALL INTELLECTUAL PROPERTY RIGHTS that waives the rest of
intellectual property rights not already waived by CC0 1.0.
The CC0 license text and ip waiver can be found in the LICENSE file.
*/

//External includes
#include <stdlib.h>
#include <string.h>
//-------------------------------------

//Internal includes
#include "../../include/SLK/SLK_types.h"
#include "../../include/SLK/SLK_functions.h"
#include "../../include/SLK/SLK_gui.h"
#include "SLK_gui_shared.h"
//-------------------------------------

//#defines
#define INSIDE(x,y,rx,ry,rw,rh) \
   ((x)>(rx)&&(x)<(rx)+(rw)&&(y)>(ry)&&(y)<(ry)+(rh))
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
//-------------------------------------

//Function prototypes
//-------------------------------------

//Function implementations

SLK_gui_window *SLK_gui_window_create(int x, int y, int width, int height)
{
   SLK_gui_window *w = malloc(sizeof(*w));
   w->pos.x = x;
   w->pos.y = y;
   w->pos.w = width;
   w->posm.w = width;
   w->pos.h = height;
   w->posm.h = height;
   w->title[0] = '\0';
   w->moveable = 0;
   w->elements = NULL;

   return w;
}

void SLK_gui_window_destroy(SLK_gui_window *w)
{
   SLK_gui_element *e = w->elements;
   while(e)
   {
      SLK_gui_element *next = e->next;
      free(e);
      e = next;
   }

   free(w);
}

void SLK_gui_window_set_title(SLK_gui_window *w, const char *title)
{
   w->title_x = 2+clip_text(w->title,title,255,(SLK_gui_rectangle){.x = 2,.y = 2,.w = w->pos.w-4,.h = 8});
}

void SLK_gui_window_draw(const SLK_gui_window *w)
{
   ///Draw frame and fill area
   SLK_draw_rgb_fill_rectangle(w->pos.x+1,w->pos.y+1,w->pos.w-2,w->pos.h-2,color_0);
   SLK_draw_rgb_vertical_line(w->pos.x,w->pos.y+1,w->pos.y+w->pos.h,color_3);
   SLK_draw_rgb_vertical_line(w->pos.x+w->pos.w-1,w->pos.y,w->pos.y+w->pos.h-1,color_1);
   SLK_draw_rgb_horizontal_line(w->pos.x+1,w->pos.x+w->pos.w-1,w->pos.y,color_1);
   SLK_draw_rgb_horizontal_line(w->pos.x+1,w->pos.x+w->pos.w-1,w->pos.y+w->pos.h-1,color_3);
   SLK_draw_rgb_color(w->pos.x,w->pos.y,color_2);
   SLK_draw_rgb_color(w->pos.x+w->pos.w-1,w->pos.y+w->pos.h-1,color_2);

   //Draw title bar
   SLK_draw_rgb_fill_rectangle(w->pos.x+2,w->pos.y+2,w->pos.w-4,10,color_1);
   SLK_draw_rgb_horizontal_line(w->pos.x+1,w->pos.x+w->pos.w-1,w->pos.y+12,color_3);
   SLK_draw_rgb_set_font_sprite(font);
   SLK_draw_rgb_string(w->pos.x+w->title_x,w->pos.y+3,1,w->title,color_5);

   //Draw elements
   SLK_gui_element *e = w->elements;
   while(e)
   {
      switch(e->type)
      {
      case SLK_GUI_ELEMENT_BUTTON:
         SLK_draw_rgb_rectangle(e->button.pos.x+w->pos.x,e->button.pos.y+w->pos.y,e->button.pos.w,e->button.pos.h,color_4);
         if(e->button.state.held)
         {
            SLK_draw_rgb_vertical_line(e->button.pos.x+1+w->pos.x,e->button.pos.y+2+w->pos.y,e->button.pos.y+2+e->button.pos.h+w->pos.y-3,color_4);
            SLK_draw_rgb_vertical_line(e->button.pos.x+1+e->button.pos.w-3+w->pos.x,e->button.pos.y+1+w->pos.y,e->button.pos.y+2+e->button.pos.h+w->pos.y-4,color_3);
            SLK_draw_rgb_horizontal_line(e->button.pos.x+2+w->pos.x,e->button.pos.x+e->button.pos.w-2+w->pos.x,e->button.pos.y+1+w->pos.y,color_3);
            SLK_draw_rgb_horizontal_line(e->button.pos.x+2+w->pos.x,e->button.pos.x+e->button.pos.w-2+w->pos.x,e->button.pos.y+1+e->button.pos.h-3+w->pos.y,color_4);
         }
         else
         {
            SLK_draw_rgb_vertical_line(e->button.pos.x+1+w->pos.x,e->button.pos.y+2+w->pos.y,e->button.pos.y+2+e->button.pos.h+w->pos.y-3,color_3);
            SLK_draw_rgb_vertical_line(e->button.pos.x+1+e->button.pos.w-3+w->pos.x,e->button.pos.y+1+w->pos.y,e->button.pos.y+2+e->button.pos.h+w->pos.y-4,color_1);
            SLK_draw_rgb_horizontal_line(e->button.pos.x+2+w->pos.x,e->button.pos.x+e->button.pos.w-2+w->pos.x,e->button.pos.y+1+w->pos.y,color_1);
            SLK_draw_rgb_horizontal_line(e->button.pos.x+2+w->pos.x,e->button.pos.x+e->button.pos.w-2+w->pos.x,e->button.pos.y+1+e->button.pos.h-3+w->pos.y,color_3);
         }
         SLK_draw_rgb_string(e->button.pos.x+e->button.text_x+w->pos.x,e->button.pos.y+w->pos.y+3,1,e->button.text,color_5);
         break;
      case SLK_GUI_ELEMENT_LABEL:
         SLK_draw_rgb_string(e->label.pos.x+e->label.text_x+w->pos.x,e->label.pos.y+w->pos.y,1,e->label.text,color_5);
         break;
      case SLK_GUI_ELEMENT_ICON:
         if(e->icon.state.held)
            SLK_draw_rgb_sprite_partial(e->icon.sprite,e->icon.pos.x+w->pos.x,e->icon.pos.y+w->pos.y,e->icon.frames[1].x,e->icon.frames[1].y,e->icon.frames[1].w,e->icon.frames[1].h);
         else
            SLK_draw_rgb_sprite_partial(e->icon.sprite,e->icon.pos.x+w->pos.x,e->icon.pos.y+w->pos.y,e->icon.frames[0].x,e->icon.frames[0].y,e->icon.frames[0].w,e->icon.frames[0].h);
         break;
      case SLK_GUI_ELEMENT_SLIDER:
            SLK_draw_rgb_fill_rectangle(e->slider.pos.x+w->pos.x,e->slider.pos.y+w->pos.y,e->slider.pos.w,e->slider.pos.h,color_4);
            if(e->slider.pos.w>e->slider.pos.h)
               SLK_draw_rgb_vertical_line(((e->slider.value-e->slider.min)/e->slider.max)*e->slider.pos.w,e->slider.pos.y+w->pos.y,e->slider.pos.y+e->slider.pos.h+w->pos.y,color_3);
            else
               SLK_draw_rgb_horizontal_line(e->slider.pos.x+w->pos.x,e->slider.pos.y+w->pos.y,((e->slider.value-e->slider.min)/e->slider.max)*e->slider.pos.h,color_3);
         break;
      }
      e = e->next;
   }

   //Draw moving preview
   if(w->moveable==2)
      SLK_draw_rgb_rectangle(w->posm.x,w->posm.y,w->pos.w,w->pos.h,color_1);
}

void SLK_gui_window_set_moveable(SLK_gui_window *w, int move)
{
   w->moveable = move;
}

void SLK_gui_window_update_input(SLK_gui_window *w, SLK_Button button_left, SLK_Button button_right, int cursor_x, int cursor_y)
{
   //Check if cursor is in window to begin with
   //goto prevents another useless if indentation
   if(!INSIDE(cursor_x,cursor_y,w->pos.x,w->pos.y,w->pos.w,w->pos.h))
      goto OUTSIDE;

   if(button_left.pressed)
   {
      //Title bar --> move window
      if(w->moveable==1&&INSIDE(cursor_x,cursor_y,w->pos.x+2,w->pos.y,w->pos.w-2,12))
      {
         w->posm.w = cursor_x-w->pos.x;
         w->posm.h = cursor_y-w->pos.y;
         w->posm.x = w->pos.x+w->posm.w;
         w->posm.y = w->pos.y+w->posm.h;
         w->moveable = 2;
      }
   }

   //Check all buttons
   SLK_gui_element *e = w->elements;
   while(e)
   {
      if(e->type==SLK_GUI_ELEMENT_BUTTON)
      {
         int status = 0;
         if(button_left.held||button_right.held)
            status = INSIDE(cursor_x,cursor_y,w->pos.x+e->button.pos.x,w->pos.y+e->button.pos.y,e->button.pos.w,e->button.pos.h);
         else
            status = 0;
         e->button.state.pressed = !e->button.state.held&&status;
         e->button.state.released = e->button.state.held&&!status;
         e->button.state.held = status;
      }
      else if(e->type==SLK_GUI_ELEMENT_ICON)
      {
         int status = 0;
         if(button_left.held||button_right.held)
            status = INSIDE(cursor_x,cursor_y,w->pos.x+e->icon.pos.x,w->pos.y+e->icon.pos.y,e->icon.pos.w,e->icon.pos.h);
         else
            status = 0;
         e->icon.state.pressed = !e->icon.state.held&&status;
         e->icon.state.released = e->icon.state.held&&!status;
         e->icon.state.held = status;
      }

      e = e->next;
   }

OUTSIDE:

   if(w->moveable==2)
   {
      if(button_left.held)
      {
         w->posm.x = -w->posm.w+cursor_x;
         w->posm.y = -w->posm.h+cursor_y;
      }
      else
      {
         w->moveable = 1;
         w->pos.x = w->posm.x;
         w->pos.y = w->posm.y;
      }
   }
}

void SLK_gui_window_add_element(SLK_gui_window *w, SLK_gui_element *e)
{
   e->next = w->elements;
   w->elements = e;
}
//-------------------------------------
