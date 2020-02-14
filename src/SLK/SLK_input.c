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
