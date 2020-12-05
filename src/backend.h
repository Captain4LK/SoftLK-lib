/*
Copyright (c) 2020, Lukas Holzbeierlein (Captain4LK) 
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _BACKEND_H_

#define _BACKEND_H_

#include "../include/SLK/SLK_types.h"

//Every backend needs to implement all of these functions. You can however just keep the implementation empty.
void backend_set_title(const char *title);
void backend_set_fullscreen(int fullscreen);
void backend_update_viewport();
void backend_set_visible(int visible);
void backend_set_icon(const SLK_RGB_sprite *icon);
int backend_get_width();
int backend_get_height();
int backend_get_view_width();
int backend_get_view_height();
int backend_get_view_x();
int backend_get_view_y();
int backend_get_win_width();
int backend_get_win_height();
void backend_set_fps(int FPS);
int backend_get_fps();
void backend_timer_update();
float backend_timer_get_delta();
void backend_handle_events();
void backend_setup(int width, int height, int layer_num, const char *title, int fullscreen, int scale, int resizable);
void backend_input_init();
void backend_show_cursor(int shown);
void backend_mouse_set_relative(int relative);
void backend_mouse_capture(int capture);
void backend_start_text_input(char *text);
void backend_stop_text_input();
void backend_render_update();
void backend_create_layer(unsigned index, int type);
int backend_key_down(int key);
int backend_key_pressed(int key);
int backend_key_released(int key);
int backend_mouse_down(int key);
int backend_mouse_pressed(int key);
int backend_mouse_released(int key);
int backend_mouse_wheel_get_scroll();
int backend_gamepad_down(int index, int key);
int backend_gamepad_pressed(int index, int key);
int backend_gamepad_released(int index, int key);
int backend_get_gamepad_count();
void backend_mouse_get_pos(int *x, int *y);
void backend_mouse_get_relative_pos(int *x, int *y);
SLK_RGB_sprite *backend_load_rgb(const char *path);
void backend_save_rgb(const SLK_RGB_sprite *s, const char *path);

#endif
