/*
   Copyright (c) 2020, Lukas Holzbeierlein (Captain4LK) 
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
       * Redistributions of source code must retain the above copyright
         notice, this list of conditions and the following disclaimer.
       * Redistributions in binary form must reproduce the above copyright
         notice, this list of conditions and the following disclaimer in the
         documentation and/or other materials provided with the distribution.
       * Neither the name of SoftLK nor the
         names of its contributors may be used to endorse or promote products
         derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL CAPTAIN4LK BE LIABLE FOR ANY
   DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
   ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _SLK_VARIABLES_H_

#define _SLK_VARIABLES_H_

#include <SDL2/SDL.h>
#include "../../include/SLK/SLK_types.h"

SLK_Pal_sprite *target_pal;
SLK_Pal_sprite *target_pal_default;
SLK_Paxel target_pal_clear;
SLK_Pal_sprite *text_sprite_pal;
SLK_RGB_sprite *target_rgb;
SLK_RGB_sprite *target_rgb_default;
SLK_Color target_rgb_clear;
SLK_RGB_sprite *text_sprite_rgb;

int screen_width;
int screen_height;
int pixel_scale;
int window_width;
int window_height;
int view_x;
int view_y;
int view_width;
int view_height;
int running;

int layer_count;
SLK_Layer *layers;
SLK_Layer *current_layer;

int mouse_x_cache;
int mouse_y_cache;

uint8_t new_key_state[256];
uint8_t old_key_state[256];
SLK_Button keyboard_state[256];
uint8_t new_mouse_state[6];
uint8_t old_mouse_state[6];
SLK_Button mouse_state[6];
uint8_t key_map[SDL_NUM_SCANCODES];
uint8_t mouse_map[6];
    
SDL_Window *sdl_window;
SDL_GLContext sdl_gl_context;
    
char *text_input;
int text_input_active;

#endif
