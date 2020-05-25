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
#include "SLK_input_i.h"
#include "SLK_render_i.h"
#include "SLK_layer_i.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
int mouse_x;
int mouse_y;
int mouse_x_rel;
int mouse_y_rel;
int mouse_wheel;
//-------------------------------------

//Function prototypes
//-------------------------------------

//Function implementations

//Returns wether the key belonging to 
//the SLK_key enum is held.
int SLK_key_down(int key)
{
   return new_key_state[key];
}

//Returns wether the key belonging to 
//the SLK_key enum has been pressed.
int SLK_key_pressed(int key)
{
   return new_key_state[key]&&!old_key_state[key];
}

//Returns wether the key belonging to 
//the SLK_key enum has been released.
int SLK_key_released(int key)
{
   return !new_key_state[key]&&old_key_state[key];
}

//Returns wether the mouse button
//refered to by SLK_mouse_button enum
//is held.
int SLK_mouse_down(int key)
{
   return new_mouse_state[key];
}

//Returns wether the mouse button
//refered to by SLK_mouse_button enum
//has been pressed.
int SLK_mouse_pressed(int key)
{
   return new_mouse_state[key]&&!old_mouse_state[key];
}

//Returns wether the mouse button
//refered to by SLK_mouse_button enum
//has been released.
int SLK_mouse_released(int key)
{
   return !new_mouse_state[key]&&old_mouse_state[key];
}

//Returns the amount the mouse wheel has been scrolled.
//Negative: towards the user.
int SLK_mouse_wheel_get_scroll()
{
   return mouse_wheel;
}

//Stores the current mouse position
//in the provided pointers.
void SLK_mouse_get_pos(int *x, int *y)
{
   *x = mouse_x;
   *y = mouse_y;
}

//Stores the mouse position relative
//to the last position in the 
//provided pointers.
void SLK_mouse_get_relative_pos(int *x, int *y)
{
   *x = mouse_x_rel;
   *y = mouse_y_rel;
}

//Gets the mouse position relative to a layer.
//Layer scaling and position are being considered.
void SLK_mouse_get_layer_pos(unsigned index, int *x, int *y)
{
   if(index<layer_count)
   {
      *x = mouse_x;
      *y = mouse_y;

      *x-=layers[index].x;
      *y-=layers[index].y;

      *x/=layers[index].scale;
      *y/=layers[index].scale;
   }
}

//Updates the mouse position (only the variable, 
//not the actual position).
//Used in SLK_update, no need to call yourself.
void SLK_i_mouse_update(int x, int y)
{
   int mouse_x_cache = mouse_x;
   int mouse_y_cache = mouse_y;
   x-=view_x;
   y-=view_y;

   mouse_x = (int)(((float)x/(float)(window_width-(view_x*2))*(float)screen_width));
   mouse_y = (int)(((float)y/(float)(window_height-(view_y*2))*(float)screen_height));
   mouse_x_rel = mouse_x-mouse_x_cache;
   mouse_y_rel = mouse_y-mouse_y_cache;

   if(mouse_x>=screen_width)
     mouse_x= screen_width-1;
   if(mouse_y>=screen_height)
     mouse_y= screen_height-1;

   if(mouse_x<0)
     mouse_x= 0;
   if(mouse_y<1)
     mouse_y= 1;
}

//Updates the mouse wheel position status.
//Used by SLK_update, no need to call yourself.
void SLK_i_mouse_update_wheel(int wheel)
{
   mouse_wheel = wheel;
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

//Internal function.
//Input init function.
//Creates keymaps and mousemaps.
void SLK_i_input_init()
{
   //Create keymaps, if I would change backends (remove SDL2, not likely though)
   //code will not break, since key names stay the same
   key_map[0x00] = SLK_KEY_NONE;
   key_map[SDL_SCANCODE_A] = SLK_KEY_A;
   key_map[SDL_SCANCODE_B] = SLK_KEY_B;
   key_map[SDL_SCANCODE_C] = SLK_KEY_C;
   key_map[SDL_SCANCODE_D] = SLK_KEY_D;
   key_map[SDL_SCANCODE_E] = SLK_KEY_E;
   key_map[SDL_SCANCODE_F] = SLK_KEY_F;
   key_map[SDL_SCANCODE_G] = SLK_KEY_G;
   key_map[SDL_SCANCODE_H] = SLK_KEY_H;
   key_map[SDL_SCANCODE_I] = SLK_KEY_I;
   key_map[SDL_SCANCODE_J] = SLK_KEY_J;
   key_map[SDL_SCANCODE_K] = SLK_KEY_K;
   key_map[SDL_SCANCODE_L] = SLK_KEY_L;
   key_map[SDL_SCANCODE_M] = SLK_KEY_M;
   key_map[SDL_SCANCODE_N] = SLK_KEY_N;
   key_map[SDL_SCANCODE_O] = SLK_KEY_O;
   key_map[SDL_SCANCODE_P] = SLK_KEY_P;
   key_map[SDL_SCANCODE_Q] = SLK_KEY_Q;
   key_map[SDL_SCANCODE_R] = SLK_KEY_R;
   key_map[SDL_SCANCODE_S] = SLK_KEY_S;
   key_map[SDL_SCANCODE_T] = SLK_KEY_T;
   key_map[SDL_SCANCODE_U] = SLK_KEY_U;
   key_map[SDL_SCANCODE_V] = SLK_KEY_V;
   key_map[SDL_SCANCODE_W] = SLK_KEY_W;
   key_map[SDL_SCANCODE_X] = SLK_KEY_X;
   key_map[SDL_SCANCODE_Y] = SLK_KEY_Y;
   key_map[SDL_SCANCODE_Z] = SLK_KEY_Z;

   key_map[SDL_SCANCODE_F1] = SLK_KEY_F1;
   key_map[SDL_SCANCODE_F2] = SLK_KEY_F2;
   key_map[SDL_SCANCODE_F3] = SLK_KEY_F3;
   key_map[SDL_SCANCODE_F4] = SLK_KEY_F4;
   key_map[SDL_SCANCODE_F5] = SLK_KEY_F5;
   key_map[SDL_SCANCODE_F6] = SLK_KEY_F6;
   key_map[SDL_SCANCODE_F7] = SLK_KEY_F7;
   key_map[SDL_SCANCODE_F8] = SLK_KEY_F8;
   key_map[SDL_SCANCODE_F9] = SLK_KEY_F9;
   key_map[SDL_SCANCODE_F10] = SLK_KEY_F10;
   key_map[SDL_SCANCODE_F11] = SLK_KEY_F11;
   key_map[SDL_SCANCODE_F12] = SLK_KEY_F12;

   key_map[SDL_SCANCODE_DOWN] = SLK_KEY_DOWN;
   key_map[SDL_SCANCODE_LEFT] = SLK_KEY_LEFT;
   key_map[SDL_SCANCODE_RIGHT] = SLK_KEY_RIGHT;
   key_map[SDL_SCANCODE_UP] = SLK_KEY_UP;
   key_map[SDL_SCANCODE_RETURN] = SLK_KEY_ENTER;

   key_map[SDL_SCANCODE_BACKSPACE] = SLK_KEY_BACK;
   key_map[SDL_SCANCODE_ESCAPE] = SLK_KEY_ESCAPE;
   key_map[SDL_SCANCODE_TAB] = SLK_KEY_TAB;
   key_map[SDL_SCANCODE_LGUI] = SLK_KEY_HOME;
   key_map[SDL_SCANCODE_END] = SLK_KEY_END;
   key_map[SDL_SCANCODE_PAGEUP] = SLK_KEY_PGUP;
   key_map[SDL_SCANCODE_PAGEDOWN] = SLK_KEY_PGDN;
   key_map[SDL_SCANCODE_INSERT] = SLK_KEY_INS;
   key_map[SDL_SCANCODE_LSHIFT] = SLK_KEY_SHIFT;
   key_map[SDL_SCANCODE_RSHIFT] = SLK_KEY_SHIFT;
   key_map[SDL_SCANCODE_LCTRL] = SLK_KEY_CTRL;
   key_map[SDL_SCANCODE_RCTRL] = SLK_KEY_CTRL;
   key_map[SDL_SCANCODE_SPACE] = SLK_KEY_SPACE;

   key_map[SDL_SCANCODE_0] = SLK_KEY_0;
   key_map[SDL_SCANCODE_1] = SLK_KEY_1;
   key_map[SDL_SCANCODE_2] = SLK_KEY_2;
   key_map[SDL_SCANCODE_3] = SLK_KEY_3;
   key_map[SDL_SCANCODE_4] = SLK_KEY_4;
   key_map[SDL_SCANCODE_5] = SLK_KEY_5;
   key_map[SDL_SCANCODE_6] = SLK_KEY_6;
   key_map[SDL_SCANCODE_7] = SLK_KEY_7;
   key_map[SDL_SCANCODE_8] = SLK_KEY_8;
   key_map[SDL_SCANCODE_9] = SLK_KEY_9;

   key_map[SDL_SCANCODE_KP_0] = SLK_KEY_NP0;
   key_map[SDL_SCANCODE_KP_1] = SLK_KEY_NP1;
   key_map[SDL_SCANCODE_KP_2] = SLK_KEY_NP2;
   key_map[SDL_SCANCODE_KP_3] = SLK_KEY_NP3;
   key_map[SDL_SCANCODE_KP_4] = SLK_KEY_NP4;
   key_map[SDL_SCANCODE_KP_5] = SLK_KEY_NP5;
   key_map[SDL_SCANCODE_KP_6] = SLK_KEY_NP6;
   key_map[SDL_SCANCODE_KP_7] = SLK_KEY_NP7;
   key_map[SDL_SCANCODE_KP_8] = SLK_KEY_NP8;
   key_map[SDL_SCANCODE_KP_9] = SLK_KEY_NP9;
   key_map[SDL_SCANCODE_KP_MULTIPLY] = SLK_KEY_NP_MUL;
   key_map[SDL_SCANCODE_KP_PLUS] = SLK_KEY_NP_ADD;
   key_map[SDL_SCANCODE_KP_DIVIDE] = SLK_KEY_NP_DIV;
   key_map[SDL_SCANCODE_KP_MINUS] = SLK_KEY_NP_SUB;
   key_map[SDL_SCANCODE_KP_PERIOD] = SLK_KEY_NP_DECIMAL;

   mouse_map[SDL_BUTTON_LEFT] = SLK_BUTTON_LEFT;
   mouse_map[SDL_BUTTON_RIGHT] = SLK_BUTTON_RIGHT;
   mouse_map[SDL_BUTTON_MIDDLE] = SLK_BUTTON_MIDDLE;
   mouse_map[SDL_BUTTON_X1] = SLK_BUTTON_X1;
   mouse_map[SDL_BUTTON_X2] = SLK_BUTTON_X2;

   //Clear key states, just in case,
   //should already be empty since known at compile time
   memset(new_key_state,0,sizeof(new_key_state));
   memset(new_mouse_state,0,sizeof(new_mouse_state));

}
//-------------------------------------
