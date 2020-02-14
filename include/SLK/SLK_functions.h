/*
(C) 2020 Lukas Holzbeierlein (Captain4LK)

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#ifndef _SLK_FUNCTIONS_H_

#define _SLK_FUNCTIONS_H_

#include "SLK_types.h"

//Palette subsystem: SLK_palette.c
SLK_Palette *SLK_palette_load(const char *path);

//Timer subsystem: SLK_timer.c
void SLK_timer_set_fps(const int FPS);
void SLK_timer_update();
float SLK_timer_get_delta();

//Render subsystem: SLK_render.c
void SLK_render_init();
void SLK_render_update();

//Pal sprite subsystem: SLK_sprite_pal.c
SLK_Pal_sprite *SLK_pal_sprite_create(const int width, const int height);
void SLK_pal_sprite_destroy(SLK_Pal_sprite *s);
SLK_Paxel SLK_pal_sprite_get_paxel(const SLK_Pal_sprite *s, const int x, const int y);
void SLK_pal_sprite_set_paxel(SLK_Pal_sprite *s, const int x, const int y, const SLK_Paxel c);
SLK_Pal_sprite *SLK_pal_sprite_load(const char *path);
void SLK_pal_sprite_save(const char *path, const SLK_Pal_sprite *s);
void SLK_pal_sprite_copy_partial(SLK_Pal_sprite *dst, const SLK_Pal_sprite *src, const int x, const int y, const int ox, const int oy, const int width, const int height);
void SLK_pal_sprite_copy(SLK_Pal_sprite *dst, const SLK_Pal_sprite *src);

//Draw pal subsystem: SLK_draw_pal.c
SLK_Pal_sprite *SLK_draw_pal_get_target();
void SLK_draw_pal_set_target(SLK_Pal_sprite *s);
void SLK_draw_pal_set_clear_paxel(SLK_Paxel paxel);
void SLK_draw_pal_clear();
void SLK_draw_pal_paxel(int x, int y, SLK_Paxel paxel);
void SLK_draw_pal_string(int x, int y, int scale, const char *text, SLK_Paxel paxel);
void SLK_draw_pal_sprite(const SLK_Pal_sprite *s, int x, int y);
void SLK_draw_pal_sprite_partial(const SLK_Pal_sprite *s, int x, int y, int ox, int oy, int width, int height);
void SLK_draw_pal_sprite_flip(const SLK_Pal_sprite *s, int x, int y, int flip);
void SLK_draw_pal_line(int x0, int y0, int x1, int y1, SLK_Paxel paxel);
void SLK_draw_pal_vertical_line(int x, int y0, int y1, SLK_Paxel paxel);
void SLK_draw_pal_horizontal_line(int x0, int x1, int y, SLK_Paxel paxel);
void SLK_draw_pal_rectangle(int x, int y, int width, int height, SLK_Paxel paxel);
void SLK_draw_pal_fill_rectangle(int x, int y, int width, int height, SLK_Paxel paxel);
void SLK_draw_pal_circle(int x, int y, int radius, SLK_Paxel paxel);
void SLK_draw_pal_fill_circle(int x, int y, int radius, SLK_Paxel paxel);

//RGB sprite subsystem: SLK_sprite_rgb.c
SLK_RGB_sprite *SLK_rgb_sprite_create(const int width, const int height);
void SLK_rgb_sprite_destroy(SLK_RGB_sprite *s);
SLK_Color SLK_rgb_sprite_get_pixel(const SLK_RGB_sprite *s, const int x, const int y);
void SLK_rgb_sprite_set_pixel(SLK_RGB_sprite *s, const int x, const int y, const SLK_Color c);
SLK_RGB_sprite *SLK_rgb_sprite_load(const char *path);
void SLK_rgb_sprite_save(const char *path, const SLK_RGB_sprite *s);

//Draw rgb subsystem: SLK_draw_rgb.c
SLK_RGB_sprite *SLK_draw_rgb_get_target();
void SLK_draw_rgb_set_target(SLK_RGB_sprite *s);
void SLK_draw_rgb_set_clear_color(SLK_Color color);
void SLK_draw_rgb_clear();
void SLK_draw_rgb_color(int x, int y, SLK_Color color);
void SLK_draw_rgb_string(int x, int y, int scale, const char *text, SLK_Color color);
void SLK_draw_rgb_sprite(const SLK_RGB_sprite *s, int x, int y);
void SLK_draw_rgb_sprite_partial(const SLK_RGB_sprite *s, int x, int y, int ox, int oy, int width, int height);
void SLK_draw_rgb_sprite_flip(const SLK_RGB_sprite *s, int x, int y, int flip);
void SLK_draw_rgb_line(int x0, int y0, int x1, int y1, SLK_Color color);
void SLK_draw_rgb_vertical_line(int x, int y0, int y1, SLK_Color color);
void SLK_draw_rgb_horizontal_line(int x0, int x1, int y, SLK_Color color);
void SLK_draw_rgb_rectangle(int x, int y, int width, int height, SLK_Color color);
void SLK_draw_rgb_fill_rectangle(int x, int y, int width, int height, SLK_Color color);
void SLK_draw_rgb_circle(int x, int y, int radius, SLK_Color color);
void SLK_draw_rgb_fill_circle(int x, int y, int radius, SLK_Color color);

//Color subsystem: SLK_color.c
SLK_Color SLK_color_create(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SLK_Paxel SLK_color_create_paxel(uint8_t index, uint8_t mask);

//Input subsystem: SLK_input.c 
int SLK_key_down(const int key);
int SLK_key_pressed(const int key);
int SLK_key_released(const int key);
int SLK_mouse_down(const int key);
int SLK_mouse_pressed(const int key);
int SLK_mouse_released(const int key);
void SLK_mouse_get_pos(int *x, int *y);
void SLK_mouse_show_cursor(int shown);
void SLK_text_input_start(char *text);
void SLK_text_input_stop();

//Layer subsystem: SLK_layer.c
void SLK_layer_create(const unsigned index, const int type);
void SLK_layer_activate(const unsigned index, const int active);
void SLK_layer_set_palette(const unsigned index, SLK_Palette *pal);
void SLK_layer_set_tint(unsigned index, SLK_Color tint);
void SLK_layer_set_current(const unsigned index);

//Core subsystem: SLK_core.c 
void SLK_setup(const int width, const int height, const int layer_num, const char *title, const int fullscreen, int scale);
void SLK_core_set_title(const char *title);
void SLK_core_set_fullscreen(int fullscreen);
void SLK_core_set_icon(const SLK_RGB_sprite *icon);
void SLK_update();
void SLK_update_viewport();
void SLK_update_mouse(int x, int y);
int SLK_running();
void SLK_quit();

/*//SLK_draw functionality
void SLK_draw_string(int x, int y, int scale, const char *text, SLK_Paxel p);
void SLK_draw_sprite(const SLK_Sprite *s, const int x, const int y);
void SLK_draw_sprite_partial(SLK_Sprite *s, int x, int y, int ox, int oy, int width, int height);
void SLK_draw_sprite_flip(const SLK_Sprite *s, const int x, const int y, const int flip);
void SLK_draw_line(int x1, int y1, int x2, int y2, SLK_Paxel p);
void SLK_draw_vertical_line(int x, int y1, int y2, SLK_Paxel p);
void SLK_draw_rectangle(int x, int y, int width, int height, SLK_Paxel p);
void SLK_fill_rectangle(int x, int y, int width, int height, SLK_Paxel p);
void SLK_clear_screen();
//--------------------------------------------

/--------------------------------------------*/

#endif //_SLK_FUNCTIONS_H_
