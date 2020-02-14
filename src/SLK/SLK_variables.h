/*
(C) 2019 Lukas Holzbeierlein (Captain4LK)

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
