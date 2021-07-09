/*
Copyright (c) 2020-2021, Lukas Holzbeierlein (Captain4LK) 
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _SLK_FUNCTIONS_H_

#define _SLK_FUNCTIONS_H_

#include <stdio.h>
#include "SLK_config.h"
#include "SLK_types.h"

//Palette subsystem: SLK_palette.c
SLK_Palette    *SLK_palette_load(const char *path);
SLK_Palette    *SLK_palette_load_file(FILE *f);
void            SLK_palette_save(const char *path, const SLK_Palette *pal);
void            SLK_palette_save_file(FILE *f, const SLK_Palette *pal);
void            SLK_palette_set_color(SLK_Palette *palette, int index, SLK_Color color);
SLK_Color       SLK_palette_get_color(const SLK_Palette *palette, int index);

//Timer subsystem: SLK_timer.c
void            SLK_timer_set_fps(int FPS);
int             SLK_timer_get_fps();
void            SLK_timer_update();
float           SLK_timer_get_delta();

//Pal sprite subsystem: SLK_sprite_pal.c
SLK_Pal_sprite *SLK_pal_sprite_create(int width, int height);
void            SLK_pal_sprite_destroy(SLK_Pal_sprite *s);
uint8_t         SLK_pal_sprite_get_index(const SLK_Pal_sprite *s, int x, int y);
void            SLK_pal_sprite_set_index(SLK_Pal_sprite *s, int x, int y, uint8_t c);
SLK_Pal_sprite *SLK_pal_sprite_load(const char *path);
SLK_Pal_sprite *SLK_pal_sprite_load_file(FILE *f);
SLK_Pal_sprite *SLK_pal_sprite_load_mem(const void *data, int length);
void            SLK_pal_sprite_save(const char *path, const SLK_Pal_sprite *s, int rle);
void            SLK_pal_sprite_save_file(FILE *f, const SLK_Pal_sprite *s, int rle);
void            SLK_pal_sprite_copy(SLK_Pal_sprite *dst, const SLK_Pal_sprite *src);
void            SLK_pal_sprite_copy_partial(SLK_Pal_sprite *dst, const SLK_Pal_sprite *src, int x, int y, int ox, int oy, int width, int height);

//Draw pal subsystem: SLK_draw_pal.c
void            SLK_draw_pal_set_target(SLK_Pal_sprite *s);
SLK_Pal_sprite *SLK_draw_pal_get_target();
void            SLK_draw_pal_set_clear_index(uint8_t index);
void            SLK_draw_pal_load_font(const char *path);
void            SLK_draw_pal_set_font_sprite(SLK_Pal_sprite *font);
void            SLK_draw_pal_clear();
void            SLK_draw_pal_index(int x, int y, uint8_t index);
void            SLK_draw_pal_string(int x, int y, int scale, const char *text, uint8_t index);
void            SLK_draw_pal_sprite(const SLK_Pal_sprite *s, int x, int y);
void            SLK_draw_pal_sprite_partial(const SLK_Pal_sprite *s, int x, int y, int ox, int oy, int width, int height);
void            SLK_draw_pal_sprite_flip(const SLK_Pal_sprite *s, int x, int y, SLK_flip flip);
void            SLK_draw_pal_line(int x0, int y0, int x1, int y1, uint8_t index);
void            SLK_draw_pal_vertical_line(int x, int y0, int y1, uint8_t index);
void            SLK_draw_pal_horizontal_line(int x0, int x1, int y, uint8_t index);
void            SLK_draw_pal_rectangle(int x, int y, int width, int height, uint8_t index);
void            SLK_draw_pal_fill_rectangle(int x, int y, int width, int height, uint8_t index);
void            SLK_draw_pal_circle(int x, int y, int radius, uint8_t index);
void            SLK_draw_pal_fill_circle(int x, int y, int radius, uint8_t index);


//RGB sprite subsystem: SLK_sprite_rgb.c
SLK_RGB_sprite *SLK_rgb_sprite_create(int width, int height);
void            SLK_rgb_sprite_destroy(SLK_RGB_sprite *s);
SLK_Color       SLK_rgb_sprite_get_pixel(const SLK_RGB_sprite *s, int x, int y);
void            SLK_rgb_sprite_set_pixel(SLK_RGB_sprite *s, int x, int y, SLK_Color c);

SLK_RGB_sprite *SLK_rgb_sprite_load(const char *path);
SLK_RGB_sprite *SLK_rgb_sprite_load_file(FILE *f);
SLK_RGB_sprite *SLK_rgb_sprite_load_mem(const void *data, int length);
void            SLK_rgb_sprite_save(const char *path, const SLK_RGB_sprite *s);
void            SLK_rgb_sprite_save_file(FILE *f, const SLK_RGB_sprite *s);
void            SLK_rgb_sprite_copy(SLK_RGB_sprite *dst, const SLK_RGB_sprite *src);
void            SLK_rgb_sprite_copy_partial(SLK_RGB_sprite *dst, const SLK_RGB_sprite *src, int x, int y, int ox, int oy, int width, int height);

//Draw rgb subsystem: SLK_draw_rgb.c
SLK_RGB_sprite *SLK_draw_rgb_get_target();
void            SLK_draw_rgb_set_target(SLK_RGB_sprite *s);
void            SLK_draw_rgb_set_clear_color(SLK_Color color);
void            SLK_draw_rgb_set_changed(int changed);
int             SLK_draw_rgb_get_changed();
void            SLK_draw_rgb_load_font(const char *path);
void            SLK_draw_rgb_set_font_sprite(SLK_RGB_sprite *font);
void            SLK_draw_rgb_clear();
void            SLK_draw_rgb_color(int x, int y, SLK_Color color);
void            SLK_draw_rgb_string(int x, int y, int scale, const char *text, SLK_Color color);
void            SLK_draw_rgb_image_string(int x, int y, int scale, const char *text);
void            SLK_draw_rgb_sprite(const SLK_RGB_sprite *s, int x, int y);
void            SLK_draw_rgb_sprite_partial(const SLK_RGB_sprite *s, int x, int y, int ox, int oy, int width, int height);
void            SLK_draw_rgb_sprite_flip(const SLK_RGB_sprite *s, int x, int y, SLK_flip flip);
void            SLK_draw_rgb_line(int x0, int y0, int x1, int y1, SLK_Color color);
void            SLK_draw_rgb_vertical_line(int x, int y0, int y1, SLK_Color color);
void            SLK_draw_rgb_horizontal_line(int x0, int x1, int y, SLK_Color color);
void            SLK_draw_rgb_rectangle(int x, int y, int width, int height, SLK_Color color);
void            SLK_draw_rgb_fill_rectangle(int x, int y, int width, int height, SLK_Color color);
void            SLK_draw_rgb_circle(int x, int y, int radius, SLK_Color color);
void            SLK_draw_rgb_fill_circle(int x, int y, int radius, SLK_Color color);

//Color subsystem: SLK_color.c
SLK_Color       SLK_color_create(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
SLK_Color       SLK_color_create_float(float r, float g, float b, float a);
SLK_Color       SLK_color_create_hsv(float h, float s, float v, float a);
SLK_Color       SLK_color_create_hsl(float h, float s, float l, float a);

//Input subsystem: SLK_input.c 
int             SLK_key_down(SLK_key key);
int             SLK_key_pressed(SLK_key key);
int             SLK_key_released(SLK_key key);
SLK_Button      SLK_key_get_state(SLK_key key);
int             SLK_gamepad_down(int id, SLK_gamepad_button key);
int             SLK_gamepad_pressed(int id, SLK_gamepad_button key);
int             SLK_gamepad_released(int id, SLK_gamepad_button key);
SLK_Button      SLK_gamepad_get_state(int id, SLK_gamepad_button key);
int             SLK_gamepad_count();
int             SLK_mouse_down(SLK_mouse_button key);
int             SLK_mouse_pressed(SLK_mouse_button key);
int             SLK_mouse_released(SLK_mouse_button key);
SLK_Button      SLK_mouse_get_state(SLK_mouse_button key);
int             SLK_mouse_wheel_get_scroll();
void            SLK_mouse_get_pos(int *x, int *y);
void            SLK_mouse_get_relative_pos(int *x, int *y);
void            SLK_mouse_get_layer_pos(unsigned index, int *x, int *y);
void            SLK_mouse_show_cursor(int shown);
void            SLK_mouse_set_relative(int relative);
void            SLK_mouse_capture(int capture);
void            SLK_text_input_start(char *text, int max_length);
void            SLK_text_input_stop();

//Layer subsystem: SLK_layer.c
void            SLK_layer_create(unsigned index, SLK_layer type);
void            SLK_layer_activate(unsigned index, int active);
void            SLK_layer_set_palette(unsigned index, SLK_Palette *pal);
void            SLK_layer_set_tint(unsigned index, SLK_Color tint);
void            SLK_layer_set_dynamic(unsigned index, int dynamic);
void            SLK_layer_set_pos(unsigned index, int x, int y);
void            SLK_layer_set_scale(unsigned index, float scale);
void            SLK_layer_set_size(unsigned index, int width, int height);
void            SLK_layer_get_size(unsigned index, int *width, int *height);
void            SLK_layer_set_current(unsigned index);
int             SLK_layer_get_resized(unsigned index);
SLK_Layer      *SLK_layer_get(unsigned index);

//Core subsystem: SLK_core.c 
void            SLK_setup(int width, int height, int layer_num, const char *title, int fullscreen, int scale, int resizable);
void            SLK_update();
void            SLK_core_set_title(const char *title);
void            SLK_core_set_fullscreen(int fullscreen);
void            SLK_core_set_visible(int visible);
void            SLK_core_set_icon(const SLK_RGB_sprite *icon);
int             SLK_core_get_width();
int             SLK_core_get_height();
int             SLK_core_running();
void            SLK_core_quit();
void            SLK_render_update();
void           *SLK_system_malloc(size_t size);
void            SLK_system_free(void *ptr);
void           *SLK_system_realloc(void *ptr, size_t size);
void            SLK_set_malloc(void *(*func)(size_t size));
void            SLK_set_free(void (*func)(void *ptr));
void            SLK_set_realloc(void *(*func)(void *ptr, size_t size));
void            SLK_log(const char *w, ...);
void            SLK_warning_file_line(const char *w, ...);
void            SLK_error_file_line(const char *e, ...);

//Log subsystem: SLK_error.c
SLK_error_pointer_type SLK_get_error_function_pointer(const char *file, int line);
SLK_error_pointer_type SLK_get_warning_function_pointer(const char *file, int line);
void SLK_default_warning(const char *st);
void SLK_default_error(const char *st);
void SLK_log(const char *w, ...);
#define SLK_error (SLK_get_error_function_pointer(__FILE__,__LINE__))
#define SLK_warning (SLK_get_warning_function_pointer(__FILE__,__LINE__))

#endif //_SLK_FUNCTIONS_H_
