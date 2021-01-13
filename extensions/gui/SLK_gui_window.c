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

#define MIN(a,b) \
   ((a)<(b)?(a):(b))

#define MAX(a,b) \
   ((a)>(b)?(a):(b))
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
//-------------------------------------

//Function prototypes

//Enables the use of recursion for drawing tabs etc.
static void gui_draw(const SLK_gui_window *w, SLK_gui_element *elements);
static void gui_input(SLK_gui_window *w, SLK_gui_element *elements, SLK_Button button_left, SLK_Button button_right, int cursor_x, int cursor_y);
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
   w->locked = 0;

   return w;
}

void SLK_gui_window_destroy(SLK_gui_window *w)
{
   free(w);
}

void SLK_gui_window_set_title(SLK_gui_window *w, const char *title)
{
   w->title_x = 2+clip_text(w->title,title,255,(SLK_gui_rectangle){.x = 2,.y = 2,.w = w->pos.w-4,.h = 8});
}

void SLK_gui_window_draw(const SLK_gui_window *w)
{
   ///Draw frame and fill area
   SLK_draw_rgb_fill_rectangle(w->pos.x+1,w->pos.y+1,w->pos.w-2,w->pos.h-2,slk_gui_color_0);
   SLK_draw_rgb_vertical_line(w->pos.x,w->pos.y+1,w->pos.y+w->pos.h,slk_gui_color_3);
   SLK_draw_rgb_vertical_line(w->pos.x+w->pos.w-1,w->pos.y,w->pos.y+w->pos.h-1,slk_gui_color_1);
   SLK_draw_rgb_horizontal_line(w->pos.x+1,w->pos.x+w->pos.w-1,w->pos.y,slk_gui_color_1);
   SLK_draw_rgb_horizontal_line(w->pos.x+1,w->pos.x+w->pos.w-1,w->pos.y+w->pos.h-1,slk_gui_color_3);
   SLK_draw_rgb_color(w->pos.x,w->pos.y,slk_gui_color_2);
   SLK_draw_rgb_color(w->pos.x+w->pos.w-1,w->pos.y+w->pos.h-1,slk_gui_color_2);

   //Draw title bar
   SLK_draw_rgb_fill_rectangle(w->pos.x+2,w->pos.y+2,w->pos.w-4,10,slk_gui_color_1);
   SLK_draw_rgb_horizontal_line(w->pos.x+1,w->pos.x+w->pos.w-1,w->pos.y+12,slk_gui_color_3);
   SLK_draw_rgb_set_font_sprite(slk_gui_font);
   SLK_draw_rgb_string(w->pos.x+w->title_x,w->pos.y+3,1,w->title,slk_gui_color_5);

   //Draw elements
   gui_draw(w,w->elements);
   
   //Draw moving preview
   if(w->moveable==2)
      SLK_draw_rgb_rectangle(w->posm.x,w->posm.y,w->pos.w,w->pos.h,slk_gui_color_1);
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
   gui_input(w,w->elements,button_left,button_right,cursor_x,cursor_y);

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

static void gui_draw(const SLK_gui_window *w, SLK_gui_element *elements)
{
   SLK_gui_element *e = elements;
   while(e)
   {
      switch(e->type)
      {
      case SLK_GUI_ELEMENT_BUTTON:
         SLK_draw_rgb_rectangle(e->button.pos.x+w->pos.x,e->button.pos.y+w->pos.y,e->button.pos.w,e->button.pos.h,slk_gui_color_4);
         if(e->button.state.held)
         {
            SLK_draw_rgb_vertical_line(e->button.pos.x+1+w->pos.x,e->button.pos.y+2+w->pos.y,e->button.pos.y+2+e->button.pos.h+w->pos.y-3,slk_gui_color_4);
            SLK_draw_rgb_vertical_line(e->button.pos.x+1+e->button.pos.w-3+w->pos.x,e->button.pos.y+1+w->pos.y,e->button.pos.y+2+e->button.pos.h+w->pos.y-4,slk_gui_color_3);
            SLK_draw_rgb_horizontal_line(e->button.pos.x+2+w->pos.x,e->button.pos.x+e->button.pos.w-2+w->pos.x,e->button.pos.y+1+w->pos.y,slk_gui_color_3);
            SLK_draw_rgb_horizontal_line(e->button.pos.x+2+w->pos.x,e->button.pos.x+e->button.pos.w-2+w->pos.x,e->button.pos.y+1+e->button.pos.h-3+w->pos.y,slk_gui_color_4);
         }
         else
         {
            SLK_draw_rgb_vertical_line(e->button.pos.x+1+w->pos.x,e->button.pos.y+2+w->pos.y,e->button.pos.y+2+e->button.pos.h+w->pos.y-3,slk_gui_color_3);
            SLK_draw_rgb_vertical_line(e->button.pos.x+1+e->button.pos.w-3+w->pos.x,e->button.pos.y+1+w->pos.y,e->button.pos.y+2+e->button.pos.h+w->pos.y-4,slk_gui_color_1);
            SLK_draw_rgb_horizontal_line(e->button.pos.x+2+w->pos.x,e->button.pos.x+e->button.pos.w-2+w->pos.x,e->button.pos.y+1+w->pos.y,slk_gui_color_1);
            SLK_draw_rgb_horizontal_line(e->button.pos.x+2+w->pos.x,e->button.pos.x+e->button.pos.w-2+w->pos.x,e->button.pos.y+1+e->button.pos.h-3+w->pos.y,slk_gui_color_3);
         }
         SLK_draw_rgb_string(e->button.pos.x+e->button.text_x+w->pos.x,e->button.pos.y+w->pos.y+3,1,e->button.text,slk_gui_color_5);
         break;
      case SLK_GUI_ELEMENT_LABEL:
         SLK_draw_rgb_string(e->label.pos.x+e->label.text_x+w->pos.x,e->label.pos.y+w->pos.y,1,e->label.text,slk_gui_color_5);
         break;
      case SLK_GUI_ELEMENT_ICON:
         if(e->icon.state.held)
            SLK_draw_rgb_sprite_partial(e->icon.sprite,e->icon.pos.x+w->pos.x,e->icon.pos.y+w->pos.y,e->icon.frames[1].x,e->icon.frames[1].y,e->icon.frames[1].w,e->icon.frames[1].h);
         else
            SLK_draw_rgb_sprite_partial(e->icon.sprite,e->icon.pos.x+w->pos.x,e->icon.pos.y+w->pos.y,e->icon.frames[0].x,e->icon.frames[0].y,e->icon.frames[0].w,e->icon.frames[0].h);
         break;
      case SLK_GUI_ELEMENT_SLIDER:
            SLK_draw_rgb_fill_rectangle(e->slider.pos.x+w->pos.x,e->slider.pos.y+w->pos.y,e->slider.pos.w,e->slider.pos.h,slk_gui_color_4);
            float t = ((float)(e->slider.value-e->slider.min)/(float)e->slider.max);
            if(e->slider.pos.w>e->slider.pos.h)
               SLK_draw_rgb_vertical_line(w->pos.x+e->slider.pos.x+t*(e->slider.pos.w-1),e->slider.pos.y+w->pos.y+1,e->slider.pos.y+e->slider.pos.h+w->pos.y-1,slk_gui_color_1);
            else
               SLK_draw_rgb_horizontal_line(e->slider.pos.x+w->pos.x+1,e->slider.pos.x+e->slider.pos.w+w->pos.x-1,w->pos.y+e->slider.pos.y+e->slider.pos.h-1-t*(e->slider.pos.h-1),slk_gui_color_1); 
         break;
      case SLK_GUI_ELEMENT_IMAGE:
         SLK_draw_rgb_sprite(e->image.sprite,e->image.pos.x+w->pos.x,e->image.pos.y+w->pos.y);
         break;
      case SLK_GUI_ELEMENT_TABBAR:
         for(int i = 0;i<e->tabbar.tabs;i++)
         {
            int tab_width = e->tabbar.pos.w/e->tabbar.tabs;
            if(i==e->tabbar.current_tab)
               SLK_draw_rgb_fill_rectangle(e->tabbar.pos.x+i*tab_width+w->pos.x,e->tabbar.pos.y+w->pos.y,tab_width,e->tabbar.pos.h,slk_gui_color_2);
            else
               SLK_draw_rgb_fill_rectangle(e->tabbar.pos.x+i*tab_width+w->pos.x,e->tabbar.pos.y+w->pos.y,tab_width,e->tabbar.pos.h,slk_gui_color_3);
            SLK_draw_rgb_string(e->tabbar.pos.x+w->pos.x+e->tabbar.tabs_text_x[i],e->tabbar.pos.y+w->pos.y+3,1,e->tabbar.tabs_text[i],slk_gui_color_5);
         }
         if(e->tabbar.elements[e->tabbar.current_tab]!=NULL)
            gui_draw(w,e->tabbar.elements[e->tabbar.current_tab]);
         break;
      case SLK_GUI_ELEMENT_VTABBAR:
         for(int i = 0;i<e->vtabbar.tabs;i++)
         {
            int tab_width = e->vtabbar.pos.w;
            int tab_height = e->vtabbar.pos.h/e->vtabbar.tabs;
            if(i==e->vtabbar.current_tab)
               SLK_draw_rgb_fill_rectangle(e->vtabbar.pos.x+w->pos.x,e->vtabbar.pos.y+w->pos.y+i*14,tab_width,tab_height,slk_gui_color_2);
            else
               SLK_draw_rgb_fill_rectangle(e->vtabbar.pos.x+w->pos.x,e->vtabbar.pos.y+w->pos.y+i*14,tab_width,tab_height,slk_gui_color_3);
            SLK_draw_rgb_string(e->vtabbar.pos.x+w->pos.x+e->vtabbar.tabs_text_x[i],e->vtabbar.pos.y+w->pos.y+3+i*14,1,e->vtabbar.tabs_text[i],slk_gui_color_5);
         }
         if(e->vtabbar.elements[e->vtabbar.current_tab]!=NULL) 
            gui_draw(w,e->vtabbar.elements[e->vtabbar.current_tab]);
         break;
      }
      e = e->next;
   }
}

static void gui_input(SLK_gui_window *w, SLK_gui_element *elements, SLK_Button button_left, SLK_Button button_right, int cursor_x, int cursor_y)
{
   SLK_gui_element *e = elements;
   while(e)
   {
      if(e->type==SLK_GUI_ELEMENT_BUTTON&&(!w->locked||e->button.selected))
      {
         int status = 0;
         if(button_left.held||button_right.held)
         {
            if((status = INSIDE(cursor_x,cursor_y,w->pos.x+e->button.pos.x,w->pos.y+e->button.pos.y,e->button.pos.w,e->button.pos.h)))
            {
               w->locked = 1;
               e->button.selected = 1;
            }
         }
         else
         {
            e->button.selected = 0;
            w->locked = 0;
            status = 0;
         }
         e->button.state.pressed = !e->button.state.held&&status;
         e->button.state.released = e->button.state.held&&!status;
         e->button.state.held = status;
      }
      else if(e->type==SLK_GUI_ELEMENT_ICON&&(!w->locked||e->icon.selected))
      {
         int status = 0;
         if(button_left.held||button_right.held)
         {
            if((status = INSIDE(cursor_x,cursor_y,w->pos.x+e->icon.pos.x,w->pos.y+e->icon.pos.y,e->icon.pos.w,e->icon.pos.h)))
            {
               w->locked = 1;
               e->icon.selected = 1;
            }
         }
         else
         {
            e->icon.selected = 0;
            w->locked = 0;
            status = 0;
         }
         e->icon.state.pressed = !e->icon.state.held&&status;
         e->icon.state.released = e->icon.state.held&&!status;
         e->icon.state.held = status;
      }
      else if(e->type==SLK_GUI_ELEMENT_SLIDER)
      {
         if(!w->locked&&INSIDE(cursor_x,cursor_y,w->pos.x+e->slider.pos.x,w->pos.y+e->slider.pos.y,e->slider.pos.w,e->slider.pos.h)&&button_left.held)
         {
            e->slider.selected = 1;
            w->locked = 1;
         }
         else if(!button_left.held)
         {
            e->slider.selected = 0;
            w->locked = 0;
         }

         if(e->slider.selected)
         {
            if(e->slider.pos.w>e->slider.pos.h)
            {
               float t = ((float)(cursor_x-w->pos.x-e->slider.pos.x)/(float)e->slider.pos.w);
               e->slider.value = e->slider.min+t*(e->slider.max-e->slider.min);
               e->slider.value = MAX(e->slider.min,MIN(e->slider.value,e->slider.max));
            }
            else
            {
               float t = ((float)(cursor_y-w->pos.y-e->slider.pos.y)/(float)e->slider.pos.h);
               e->slider.value = e->slider.max-(e->slider.min+t*(e->slider.max-e->slider.min));
               e->slider.value = MAX(e->slider.min,MIN(e->slider.max,e->slider.value));
            }
         }
      }
      else if(e->type==SLK_GUI_ELEMENT_TABBAR)
      {
         if(button_left.pressed&&INSIDE(cursor_x,cursor_y,w->pos.x+e->tabbar.pos.x,w->pos.y+e->tabbar.pos.y,e->tabbar.pos.w,e->tabbar.pos.h))
            e->tabbar.current_tab = (cursor_x-w->pos.x-e->tabbar.pos.x)/(e->tabbar.pos.w/e->tabbar.tabs);
         if(e->tabbar.elements[e->tabbar.current_tab]!=NULL)
            gui_input(w,e->tabbar.elements[e->tabbar.current_tab],button_left,button_right,cursor_x,cursor_y);
      }
      else if(e->type==SLK_GUI_ELEMENT_VTABBAR)
      {
         int height = 14*e->vtabbar.tabs;
         if(button_left.pressed&&INSIDE(cursor_x,cursor_y,w->pos.x+e->vtabbar.pos.x,w->pos.y+e->vtabbar.pos.y,e->vtabbar.pos.w,height))
            e->vtabbar.current_tab = (cursor_y-w->pos.y-e->vtabbar.pos.y)/(height/e->vtabbar.tabs);
         if(e->vtabbar.elements[e->vtabbar.current_tab]!=NULL)
            gui_input(w,e->vtabbar.elements[e->vtabbar.current_tab],button_left,button_right,cursor_x,cursor_y);
      }

      e = e->next;
   }
}
//-------------------------------------
