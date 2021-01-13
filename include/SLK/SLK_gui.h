/*
SLK_gui - gui extension for SoftLK-lib

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

This work's goal is to never be encumbered by any exclusive intellectual
property rights. The work is therefore provided under CC0 1.0 + additional
WAIVER OF ALL INTELLECTUAL PROPERTY RIGHTS that waives the rest of
intellectual property rights not already waived by CC0 1.0.
The CC0 license text and ip waiver can be found in the LICENSE file.
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
   char text_x;
   int selected;
}SLK_gui_button;

typedef struct
{
   SLK_gui_rectangle pos;
   char text[512];
   char text_x;
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
   char title_x;

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
