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
#include <math.h>
//-------------------------------------

//Internal includes
#include "../../include/SLK/SLK_types.h"
#include "../../include/SLK/SLK_functions.h"
#include "../../include/SLK/SLK_gui.h"
#include "SLK_gui_shared.h"
//-------------------------------------

//#defines
//Enable if you prefer bilinear scaling for image elements
#define BILINEAR 0
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
//-------------------------------------

//Function prototypes
//-------------------------------------

//Function implementations

SLK_gui_element *SLK_gui_button_create(int x, int y, int width, int height, const char *label)
{
   SLK_gui_element *e = malloc(sizeof(*e));
   e->next = NULL;
   e->type = SLK_GUI_ELEMENT_BUTTON;
   e->button.pos.x = x;
   e->button.pos.y = y;
   e->button.pos.w = width;
   e->button.pos.h = height;
   e->button.state.held = 0;
   e->button.state.pressed = 0;
   e->button.state.released = 0;
   e->button.text_x = 2+clip_text(e->button.text,label,255,(SLK_gui_rectangle){.x = 2,.y = 2,.w = width-4,.h = height-4});
   e->button.selected = 0;

   return e;
}

SLK_gui_element *SLK_gui_label_create(int x, int y, int width, int height, const char *label)
{
   SLK_gui_element *e = malloc(sizeof(*e));
   e->next = NULL;
   e->type = SLK_GUI_ELEMENT_LABEL;
   e->label.pos.x = x;
   e->label.pos.y = y;
   e->label.pos.w = width;
   e->label.pos.h = height;
   e->label.text_x = 2+clip_text(e->label.text,label,255,(SLK_gui_rectangle){.x = 2,.y = 2,.w = width-4,.h = height-4});

   return e;
}

void SLK_gui_label_set_text(SLK_gui_element *element, const char *label)
{
   if(element->type!=SLK_GUI_ELEMENT_LABEL)
      return;

   element->label.text_x = 2+clip_text(element->label.text,label,255,(SLK_gui_rectangle){.x = 2,.y = 2,.w = element->label.pos.w-4,.h = element->label.pos.h-4});
}

SLK_gui_element *SLK_gui_icon_create(int x, int y, int width, int height, SLK_RGB_sprite *sprite, SLK_gui_rectangle frame_up, SLK_gui_rectangle frame_down)
{
   SLK_gui_element *e = malloc(sizeof(*e));
   e->next = NULL;
   e->type = SLK_GUI_ELEMENT_ICON;
   e->icon.sprite = sprite;
   e->icon.pos.x = x;
   e->icon.pos.y = y;
   e->icon.pos.w = width;
   e->icon.pos.h = height;
   e->icon.frames[0] = frame_up;
   e->icon.frames[1] = frame_down;
   e->icon.state.held = 0;
   e->icon.state.pressed = 0;
   e->icon.state.released = 0;
   e->icon.selected = 0;

   return e;
}

SLK_gui_element *SLK_gui_slider_create(int x, int y, int width, int height, int min, int max)
{
   SLK_gui_element *e = malloc(sizeof(*e));
   e->next = NULL;
   e->type = SLK_GUI_ELEMENT_SLIDER;
   e->slider.pos.x = x;
   e->slider.pos.y = y;
   e->slider.pos.w = width;
   e->slider.pos.h = height;
   e->slider.min = min;
   e->slider.max = max;
   e->slider.value = min;
   e->slider.selected = 0;
   
   return e;
}

SLK_gui_element *SLK_gui_image_create(int x, int y, int width, int height, SLK_RGB_sprite *sprite, SLK_gui_rectangle frame)
{
   SLK_gui_element *e = malloc(sizeof(*e));
   e->next = NULL;
   e->type = SLK_GUI_ELEMENT_IMAGE;
   e->image.sprite = SLK_rgb_sprite_create(width,height);
   e->image.pos.x = x;
   e->image.pos.y = y;
   e->image.pos.w = width;
   e->image.pos.h = height;
   SLK_gui_image_update(e,sprite,frame);

   return e;
}

void SLK_gui_image_update(SLK_gui_element *element, SLK_RGB_sprite *sprite, SLK_gui_rectangle frame)
{
   //Sample image
   int width = element->image.pos.w;
   int height = element->image.pos.h;
   SLK_RGB_sprite *old = SLK_draw_rgb_get_target();
   SLK_draw_rgb_set_target(element->image.sprite);
   SLK_draw_rgb_set_clear_color(slk_gui_color_0);
   SLK_draw_rgb_clear();

   //Special case: Image fits perfectly
   if(frame.w==width&&frame.h<=height)
   {
      int iy = (height-frame.h)/2; 
      SLK_rgb_sprite_copy_partial(element->image.sprite,sprite,0,iy,frame.x,frame.y,frame.w,frame.h);
   }
   else if(frame.h==height&&frame.w<=width)
   {
      int ix = (width-frame.w)/2; 
      SLK_rgb_sprite_copy_partial(element->image.sprite,sprite,ix,0,frame.x,frame.y,frame.w,frame.h);
   }
   else
   {
      int fwidth;
      int fheight;
      if(frame.w>frame.h)
      {
         fwidth = width;
         fheight = ((float)frame.h/(float)frame.w)*height;
      }
      else
      {
         fheight = height;
         fwidth = ((float)frame.w/(float)frame.h)*width;
      }
      int ix = (width-fwidth)/2;
      int iy = (height-fheight)/2;

      for(int sx = 0;sx<fwidth;sx++)
      {
         for(int sy = 0;sy<fheight;sy++)
         {
#if BILINEAR 

            double px = ((double)sx/(double)fwidth)*(double)sprite->width;
            double py = ((double)sy/(double)fheight)*(double)sprite->height;
            double pix = ((double)sx/(double)fwidth);
            double piy = ((double)sy/(double)fheight);

            SLK_Color c;
            SLK_Color c1,c2,c3,c4;
            c1 = SLK_rgb_sprite_get_pixel(sprite,floor(px),floor(py));
            c2 = SLK_rgb_sprite_get_pixel(sprite,ceil(px),floor(py));
            c3 = SLK_rgb_sprite_get_pixel(sprite,floor(px),ceil(py));
            c4 = SLK_rgb_sprite_get_pixel(sprite,ceil(px),ceil(py));
            float c1t = ((1.0f-pix)*(float)c1.r+pix*(float)c2.r);
            float c2t = ((1.0f-pix)*(float)c3.r+pix*(float)c4.r);
            c.r = (int)((1.0f-piy)*c1t+piy*c2t);
            c1t = ((1.0f-pix)*(float)c1.g+pix*(float)c2.g);
            c2t = ((1.0f-pix)*(float)c3.g+pix*(float)c4.g);
            c.g = (int)((1.0f-piy)*c1t+piy*c2t);
            c1t = ((1.0f-pix)*(float)c1.b+pix*(float)c2.b);
            c2t = ((1.0f-pix)*(float)c3.b+pix*(float)c4.b);
            c.b = (int)((1.0f-piy)*c1t+piy*c2t);
            c1t = ((1.0f-pix)*(float)c1.a+pix*(float)c2.a);
            c2t = ((1.0f-pix)*(float)c3.a+pix*(float)c4.a);
            c.a = (int)((1.0f-piy)*c1t+piy*c2t);
            SLK_rgb_sprite_set_pixel(element->image.sprite,sx+ix,sy+iy,c);
#else

            SLK_Color c = SLK_rgb_sprite_get_pixel(sprite,((float)sx/(float)fwidth)*sprite->width,((float)sy/(float)fheight)*sprite->height);
            SLK_rgb_sprite_set_pixel(element->image.sprite,sx+ix,sy+iy,c);

#endif
         }
      }
   }
   SLK_draw_rgb_set_target(old);
}

SLK_gui_element *SLK_gui_tabbar_create(int x, int y, int width, int height, int tab_count, const char **tabs_text)
{
   int tab_width = width/tab_count;
   SLK_gui_element *e = malloc(sizeof(*e));
   e->next = NULL;
   e->type = SLK_GUI_ELEMENT_TABBAR;
   e->tabbar.elements = malloc(sizeof(*e->tabbar.elements)*tab_count);;
   e->tabbar.pos.x = x;
   e->tabbar.pos.y = y;
   e->tabbar.pos.w = width;
   e->tabbar.pos.h = height;
   e->tabbar.current_tab = 0;
   e->tabbar.tabs = tab_count;
   e->tabbar.tabs_text = malloc(sizeof(*e->tabbar.tabs_text)*tab_count);
   e->tabbar.tabs_text_x = malloc(sizeof(*e->tabbar.tabs_text_x)*tab_count);
   for(int i = 0;i<tab_count;i++)
   {
      e->tabbar.elements[i] = NULL;
      e->tabbar.tabs_text[i] = malloc(sizeof(**e->tabbar.tabs_text)*256);
      e->tabbar.tabs_text_x[i] = clip_text(e->tabbar.tabs_text[i],tabs_text[i],256,(SLK_gui_rectangle){0,0,tab_width,height})+i*tab_width;
   }

   return e;
}

void SLK_gui_tabbar_add_element(SLK_gui_element *bar, int tab, SLK_gui_element *element_new)
{
   element_new->next = bar->tabbar.elements[tab];
   bar->tabbar.elements[tab] = element_new;
}

SLK_gui_element *SLK_gui_vtabbar_create(int x, int y, int width, int tab_count, const char **tabs_text)
{
   int tab_width = width;
   SLK_gui_element *e = malloc(sizeof(*e));
   e->next = NULL;
   e->type = SLK_GUI_ELEMENT_VTABBAR;
   e->tabbar.elements = malloc(sizeof(*e->tabbar.elements)*tab_count);;
   e->tabbar.pos.x = x;
   e->tabbar.pos.y = y;
   e->tabbar.pos.w = width;
   e->tabbar.pos.h = 14*tab_count;
   e->tabbar.current_tab = 0;
   e->tabbar.tabs = tab_count;
   e->tabbar.tabs_text = malloc(sizeof(*e->tabbar.tabs_text)*tab_count);
   e->tabbar.tabs_text_x = malloc(sizeof(*e->tabbar.tabs_text_x)*tab_count);
   for(int i = 0;i<tab_count;i++)
   {
      e->tabbar.elements[i] = NULL;
      e->tabbar.tabs_text[i] = malloc(sizeof(**e->tabbar.tabs_text)*256);
      e->tabbar.tabs_text_x[i] = clip_text(e->tabbar.tabs_text[i],tabs_text[i],256,(SLK_gui_rectangle){0,0,tab_width,14});
   }

   return e;
}

void SLK_gui_vtabbar_add_element(SLK_gui_element *bar, int tab, SLK_gui_element *element_new)
{
   element_new->next = bar->tabbar.elements[tab];
   bar->vtabbar.elements[tab] = element_new;
}
//-------------------------------------
