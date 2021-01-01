/*
Copyright (c) 2020-2021, Lukas Holzbeierlein (Captain4LK) 
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//External includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//-------------------------------------

//Internal includes
#include "../../include/SLK/SLK_types.h"
#include "../../include/SLK/SLK_functions.h"
#include "../SLK_layer_i.h"
#include "../backend.h"
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

//Set the window title.
void backend_set_title(const char *title)
{

}

//Toggles fullscreen.
void backend_set_fullscreen(int fullscreen)
{

}

//(should) center the viewport.
void backend_update_viewport()
{

}

//Sets wether the window is visible.
void backend_set_visible(int visible)
{

}

//Sets the window icon.
void backend_set_icon(const SLK_RGB_sprite *icon)
{

}

//Returns the viewport width adjusted to pixel scale.
int backend_get_width()
{
   return 0;
}

//Returns the viewport height adjusted to pixel scale.
int backend_get_height()
{
   return 0;
}

//Returns the view width.
int backend_get_view_width()
{
   return 0;
}

//Returns the view height.
int backend_get_view_height()
{
   return 0;
}

//Returns the view x pos.
int backend_get_view_x()
{
   return 0;
}

//Returns the view y pos.
int backend_get_view_y()
{
   return 0;
}

//Returns the window width.
int backend_get_win_width()
{
   return 0;
}

//Returns the window height.
int backend_get_win_height()
{
   return 0;
}

//Sets the target/maximum fps.
void backend_set_fps(int FPS)
{

}

//Returns the current target fps.
int backend_get_fps()
{
   return 0;
}

//Limits the fps to the target fps.
void backend_timer_update()
{

}

//Returns the delta time of the last frame.
float backend_timer_get_delta()
{
   return 0.0f;
}

//Handles window and input events.
void backend_handle_events()
{

}

//Creates the window, etc.
void backend_setup(int width, int height, int layer_num, const char *title, int fullscreen, int scale, int resizable)
{

}

//Creates the keymap.
void backend_input_init()
{

}

//Shows or hides the mouse cursor.
void backend_show_cursor(int shown)
{

}

//Sets wether the mouse cursor is captured and only relative
//mouse motion is registerd.
void backend_mouse_set_relative(int relative)
{

}

//Sets wether to track mouse events globally.
void backend_mouse_capture(int capture)
{

}

//Starts text input.
void backend_start_text_input(char *text)
{

}

//Stops text input.
void backend_stop_text_input()
{

}


//Clears the window and redraws the scene.
//Drawing is performed from back to front, layer 0 is always drawn last.
void backend_render_update()
{

}

void backend_create_layer(unsigned index, int type)
{
   
}

int backend_key_down(int key)
{
   return 0;
}

int backend_key_pressed(int key)
{
   return 0;
}

int backend_key_released(int key)
{
   return 0;
}

int backend_mouse_down(int key)
{
   return 0;
}

int backend_mouse_pressed(int key)
{
   return 0;
}

int backend_mouse_released(int key)
{
   return 0;
}

int backend_mouse_wheel_get_scroll()
{
   return 0;
}

int backend_gamepad_down(int index, int key)
{
   return 0;
}

int backend_gamepad_pressed(int index, int key)
{
   return 0;
}

int backend_gamepad_released(int index, int key)
{
   return 0;
}

int backend_get_gamepad_count()
{
   return 0;
}

void backend_mouse_get_pos(int *x, int *y)
{

}

void backend_mouse_get_relative_pos(int *x, int *y)
{

}

SLK_RGB_sprite *backend_load_rgb(const char *path)
{
   return NULL;
}

SLK_RGB_sprite *backend_load_rgb_file(FILE *f)
{
   return NULL;
}

SLK_RGB_sprite *backend_load_rgb_mem(const void *data, int length)
{
   return NULL;
}

void backend_save_rgb(const SLK_RGB_sprite *s, const char *path)
{

}

void backend_save_rgb_file(const SLK_RGB_sprite *s, FILE *f)
{

}

SLK_Pal_sprite *backend_load_pal(const char *path)
{
   return NULL;
}

SLK_Pal_sprite *backend_load_pal_file(FILE *f)
{
   return NULL;
}

SLK_Pal_sprite *backend_load_pal_mem(const void *data, int length)
{
   return NULL;
}

void backend_save_pal(const SLK_Pal_sprite *s, const char *path, int rle)
{

}

void backend_save_pal_file(const SLK_Pal_sprite *s, FILE *f, int rle)
{

}

SLK_Palette *backend_load_palette(const char *path)
{
   return NULL;
}

void backend_set_main_loop(void (*callback)(void))
{

}
//-------------------------------------
