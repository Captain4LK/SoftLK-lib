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
//-------------------------------------

//Internal includes
#include "../include/SLK/SLK_types.h"
#include "backend.h"
#include "SLK_layer_i.h"
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
   return backend_key_down(key);
}

//Returns wether the key belonging to 
//the SLK_key enum has been pressed.
int SLK_key_pressed(int key)
{
   return backend_key_pressed(key);
}

//Returns wether the key belonging to 
//the SLK_key enum has been released.
int SLK_key_released(int key)
{
   return backend_key_released(key);
}

//Returns wether the button belonging to 
//the SLK_button enum is held on the specified gamepad.
int SLK_gamepad_down(int id, int key)
{
   return backend_gamepad_down(id,key);
}

//Returns wether the button belonging to 
//the SLK_button enum has been pressed on the specified gamepad.
int SLK_gamepad_pressed(int id, int key)
{
   return backend_gamepad_pressed(id,key);
}

//Returns wether the button belonging to 
//the SLK_button enum has been released on the specified gamepad.
int SLK_gamepad_released(int id, int key)
{
   return backend_gamepad_released(id,key);
}

//Returns how many gamepads are currently plugged in.
int SLK_gamepad_count()
{
   return backend_get_gamepad_count();
}

//Returns wether the mouse button
//refered to by SLK_mouse_button enum
//is held.
int SLK_mouse_down(int key)
{
   return backend_mouse_down(key);
}

//Returns wether the mouse button
//refered to by SLK_mouse_button enum
//has been pressed.
int SLK_mouse_pressed(int key)
{
   return backend_mouse_pressed(key);
}

//Returns wether the mouse button
//refered to by SLK_mouse_button enum
//has been released.
int SLK_mouse_released(int key)
{
   return backend_mouse_released(key);
}

//Returns the amount the mouse wheel has been scrolled.
//Negative: towards the user.
int SLK_mouse_wheel_get_scroll()
{
   return backend_mouse_wheel_get_scroll();
}

//Stores the current mouse position
//in the provided pointers.
void SLK_mouse_get_pos(int *x, int *y)
{
   backend_mouse_get_pos(x,y);
}

//Stores the mouse position relative
//to the last position in the 
//provided pointers.
void SLK_mouse_get_relative_pos(int *x, int *y)
{
   backend_mouse_get_relative_pos(x,y);
}

//Gets the mouse position relative to a layer.
//Layer scaling and position are being considered.
void SLK_mouse_get_layer_pos(unsigned index, int *x, int *y)
{
   int mouse_x, mouse_y;
   SLK_mouse_get_pos(&mouse_x,&mouse_y);

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

//Sets wether the cursor should be shown.
void SLK_mouse_show_cursor(int shown)
{
   backend_show_cursor(shown);
}

//Sets wether the mouse cursor is captured and only relative
//mouse motion is registerd.
void SLK_mouse_set_relative(int relative)
{
   backend_mouse_set_relative(relative);
}

//Sets wether to capture mouse events globally.
void SLK_mouse_capture(int capture)
{
   backend_mouse_capture(capture);
}

//Starts text ínput and appends the characters
//to the provided char pointer.
//Note: SoftLK does not allocate new memory,
//you need to do that yourself.
void SLK_text_input_start(char *text)
{
   backend_start_text_input(text);
}

//Stops the text input.
void SLK_text_input_stop()
{
   backend_stop_text_input();
}
//-------------------------------------
