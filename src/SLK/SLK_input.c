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
       * Neither the name of SLK nor the
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

#include "../../include/SLK/SLK.h"
#include "SLK_variables.h"

int SLK_key_down(const int key)
{
   return keyboard_state[key].held;
}

int SLK_key_pressed(const int key)
{
   return keyboard_state[key].pressed;
}

int SLK_key_released(const int key)
{
   return keyboard_state[key].released;
}

int SLK_mouse_down(const int key)
{
   return mouse_state[key].held;
}

int SLK_mouse_pressed(const int key)
{
   return mouse_state[key].pressed;
}

int SLK_mouse_released(const int key)
{
   return mouse_state[key].released;
}

void SLK_mouse_get_pos(int *x, int *y)
{
   *x = mouse_x_cache;
   *y = mouse_y_cache;
}

void SLK_mouse_show_cursor(int shown)
{
   SDL_ShowCursor(shown?SDL_ENABLE:SDL_DISABLE);
}

void SLK_text_input_start(char *text)
{
   text_input = text;
   text_input_active = 1;

   SDL_StartTextInput();
}

void SLK_text_input_stop()
{
   text_input_active = 0;

   SDL_StopTextInput();
}
