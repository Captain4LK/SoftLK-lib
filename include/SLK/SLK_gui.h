/* 
LICENSE OPTION A: 3-clause BSD

   Copyright (C) 2021 Captain4LK (Lukas Holzbeierlein)

   Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

   3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

LICENSE OPTION B: Public Domain - Unlicense

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

#ifndef _SLK_GUI_H_

#define _SLK_GUI_H_

#include "../../include/SLK/SLK_types.h"

typedef struct SLK_gui_element SLK_gui_element;

typedef struct
{
   int x;
   int y;
   int w;
   int h;
}SLK_gui_rectangle;

enum SLK_gui_element_type
{
   SLK_GUI_ELEMENT_BUTTON, SLK_GUI_ELEMENT_LABEL,
   SLK_GUI_ELEMENT_ICON, SLK_GUI_ELEMENT_SLIDER,
   SLK_GUI_ELEMENT_IMAGE, SLK_GUI_ELEMENT_TABBAR,
   SLK_GUI_ELEMENT_VTABBAR,
};

typedef struct
{
   SLK_gui_rectangle pos;
   SLK_Button state;
   char text[256];
   int text_x;
   int selected;
}SLK_gui_button;

typedef struct
{
   SLK_gui_rectangle pos;
   char text[512];
   int text_x;
}SLK_gui_label;

typedef struct
{
   SLK_gui_rectangle pos;
   SLK_gui_rectangle frames[2];
   SLK_RGB_sprite *sprite;
   SLK_Button state;
   int selected;
}SLK_gui_icon;

typedef struct
{
   SLK_gui_rectangle pos;
   int value; //Supposed to be accessed by user
   int min;
   int max;
   int selected;
}SLK_gui_slider;

typedef struct
{
   SLK_gui_rectangle pos;
   SLK_RGB_sprite *sprite;
}SLK_gui_image;

typedef struct
{
   SLK_gui_rectangle pos;
   int current_tab;
   int tabs;
   char **tabs_text;
   int *tabs_text_x;

   SLK_gui_element **elements;
}SLK_gui_tabbar;

typedef struct
{
   SLK_gui_rectangle pos;
   int current_tab;
   int tabs;
   char **tabs_text;
   int *tabs_text_x;

   SLK_gui_element **elements;
}SLK_gui_vtabbar;

typedef struct SLK_gui_element
{
   int type;

   union
   {
      SLK_gui_button button;
      SLK_gui_label label;
      SLK_gui_icon icon;
      SLK_gui_slider slider;
      SLK_gui_image image;
      SLK_gui_tabbar tabbar;
      SLK_gui_vtabbar vtabbar;
   };

   struct SLK_gui_element *next;
}SLK_gui_element;

typedef struct
{
   SLK_gui_rectangle pos;
   SLK_gui_rectangle posm;
   int moveable;
   int locked;
   char title[256];
   int title_x;

   SLK_gui_element *elements;
}SLK_gui_window;

SLK_gui_window  *SLK_gui_window_create(int x, int y, int width, int height); //Position can be changed, width and height are constants
void             SLK_gui_window_destroy(SLK_gui_window *w);
void             SLK_gui_window_set_title(SLK_gui_window *w, const char *title);
void             SLK_gui_window_draw(const SLK_gui_window *w);
void             SLK_gui_window_set_moveable(SLK_gui_window *w, int move);
void             SLK_gui_window_update_input(SLK_gui_window *w, SLK_Button button_left, SLK_Button button_right, int cursor_x, int cursor_y);
void             SLK_gui_window_add_element(SLK_gui_window *w, SLK_gui_element *e);

SLK_gui_element *SLK_gui_button_create(int x, int y, int width, int height, const char *label);
SLK_gui_element *SLK_gui_icon_create(int x, int y, int width, int height, SLK_RGB_sprite *sprite, SLK_gui_rectangle frame_up, SLK_gui_rectangle frame_down);
SLK_gui_element *SLK_gui_label_create(int x, int y, int width, int height, const char *label);
void             SLK_gui_label_set_text(SLK_gui_element *element, const char *label);
SLK_gui_element *SLK_gui_slider_create(int x, int y, int width, int height, int min, int max);
SLK_gui_element *SLK_gui_image_create(int x, int y, int width, int height, SLK_RGB_sprite *sprite, SLK_gui_rectangle frame);
void             SLK_gui_image_update(SLK_gui_element *element, SLK_RGB_sprite *sprite, SLK_gui_rectangle frame);
SLK_gui_element *SLK_gui_tabbar_create(int x, int y, int width, int height, int tab_count, const char **tabs_text);
void             SLK_gui_tabbar_add_element(SLK_gui_element *bar, int tab, SLK_gui_element *element_new);
SLK_gui_element *SLK_gui_vtabbar_create(int x, int y, int width, int tab_count, const char **tabs_text);
void             SLK_gui_vtabbar_add_element(SLK_gui_element *bar, int tab, SLK_gui_element *element_new);

void             SLK_gui_set_font(SLK_RGB_sprite *f);
SLK_RGB_sprite  *SLK_gui_get_font();
void             SLK_gui_set_font_color(SLK_Color color);
void             SLK_gui_set_colors(SLK_Color c0, SLK_Color c1, SLK_Color c2, SLK_Color c3, SLK_Color c4);

#endif
