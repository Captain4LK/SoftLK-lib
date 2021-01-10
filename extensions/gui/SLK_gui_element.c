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
//-------------------------------------