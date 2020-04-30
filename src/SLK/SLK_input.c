/*
Copyright (c) 2020, Lukas Holzbeierlein (Captain4LK) 
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//External includes
#include <SDL2/SDL.h>
//-------------------------------------

//Internal includes
#include "../../include/SLK/SLK_types.h"
#include "SLK_variables.h"
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

//Returns wether the key belonging to 
//the SLK_key enum is held.
int SLK_key_down(int key)
{
   return keyboard_state[key].held;
}

//Returns wether the key belonging to 
//the SLK_key enum has been pressed.
int SLK_key_pressed(int key)
{
   return keyboard_state[key].pressed;
}

//Returns wether the key belonging to 
//the SLK_key enum has been released.
int SLK_key_released(int key)
{
   return keyboard_state[key].released;
}

//Returns wether the mouse button
//refered to by SLK_mouse_button enum
//is held.
int SLK_mouse_down(int key)
{
   return mouse_state[key].held;
}

//Returns wether the mouse button
//refered to by SLK_mouse_button enum
//has been pressed.
int SLK_mouse_pressed(int key)
{
   return mouse_state[key].pressed;
}

//Returns wether the mouse button
//refered to by SLK_mouse_button enum
//has been released.
int SLK_mouse_released(int key)
{
   return mouse_state[key].released;
}

//Stores the current mouse position
//in the provided pointers.
void SLK_mouse_get_pos(int *x, int *y)
{
   *x = mouse_x_cache;
   *y = mouse_y_cache;
}

//Updates the mouse position (only the variable, 
//not the actual position).
//Used in SLK_update, no need to call yourself.
void SLK_mouse_update(int x, int y)
{
    x-=view_x;
    y-=view_y;

    mouse_x_cache = (int)(((float)x/(float)(window_width-(view_x*2))*(float)screen_width));
    mouse_y_cache = (int)(((float)y/(float)(window_height-(view_y*2))*(float)screen_height));

    if(mouse_x_cache>=screen_width)
        mouse_x_cache = screen_width-1;
    if(mouse_y_cache>=screen_height)
        mouse_y_cache = screen_height-1;

    if(mouse_x_cache<0)
        mouse_x_cache = 0;
    if(mouse_y_cache<1)
        mouse_y_cache = 1;
}

//Sets wether the cursor should be shown.
void SLK_mouse_show_cursor(int shown)
{
   SDL_ShowCursor(shown?SDL_ENABLE:SDL_DISABLE);
}

//Starts text ínput and appends the characters
//to the provided char pointer.
//Note: SoftLK does not allocate new memory,
//you need to do that yourself.
void SLK_text_input_start(char *text)
{
   text_input = text;
   text_input_active = 1;

   SDL_StartTextInput();
}

//Stops the text input.
void SLK_text_input_stop()
{
   text_input_active = 0;

   SDL_StopTextInput();
}
//-------------------------------------
