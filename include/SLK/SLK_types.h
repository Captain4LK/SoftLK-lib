/*
Copyright (c) 2020, Lukas Holzbeierlein (Captain4LK) 
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _SLK_TYPES_H_

#define _SLK_TYPES_H_

#include <stdint.h>

typedef struct
{
   union
   {
      uint32_t n;
      struct
      {
         uint8_t r;
         uint8_t g;
         uint8_t b;
         uint8_t a;
      };
   };
}SLK_Color;

typedef struct
{
   uint8_t index;
   uint8_t mask;
}SLK_Paxel;

typedef struct
{
    SLK_Color colors[256];
}SLK_Palette;

typedef struct
{
    int width;
    int height;
    int changed;
    SLK_Color *data;
}SLK_RGB_sprite;

typedef struct
{
   int width;
   int height;
   int changed;
   SLK_Paxel *data;
}SLK_Pal_sprite;

typedef struct
{
   int size;
   SLK_RGB_sprite **sheet;
}SLK_RGB_sheet;

typedef struct
{
   int size;
   SLK_Pal_sprite **sheet;
}SLK_Pal_sheet;

typedef struct
{
   SLK_Pal_sprite *target;
   SLK_RGB_sprite *render;
   SLK_Palette *palette;
}SLK_Layer_pal;

typedef struct
{
   SLK_RGB_sprite *target;
}SLK_Layer_rgb;

typedef struct
{
   int type;
   int active;
   int dynamic;
   int resized;
   int x;
   int y;
   float scale;
   SLK_Color tint;

   union
   {
      SLK_Layer_pal type_0;
      SLK_Layer_rgb type_1;
   };
}SLK_Layer;

typedef struct
{
   int pressed;
   int released;
   int held;
}SLK_Button;

enum SLK_key
{
   SLK_KEY_NONE,
   SLK_KEY_A,SLK_KEY_B,SLK_KEY_C,SLK_KEY_D,SLK_KEY_E,SLK_KEY_F,SLK_KEY_G,
   SLK_KEY_H,SLK_KEY_I,SLK_KEY_J,SLK_KEY_K,SLK_KEY_L,SLK_KEY_M,SLK_KEY_N,
   SLK_KEY_O,SLK_KEY_P,SLK_KEY_Q,SLK_KEY_R,SLK_KEY_S,SLK_KEY_T,SLK_KEY_U,
   SLK_KEY_V,SLK_KEY_W,SLK_KEY_X,SLK_KEY_Y,SLK_KEY_Z,
   SLK_KEY_0,SLK_KEY_1,SLK_KEY_2,SLK_KEY_3,SLK_KEY_4,SLK_KEY_5,
   SLK_KEY_6,SLK_KEY_7,SLK_KEY_8,SLK_KEY_9,
   SLK_KEY_F1,SLK_KEY_F2,SLK_KEY_F3,SLK_KEY_F4,SLK_KEY_F5,SLK_KEY_F6,
   SLK_KEY_F7,SLK_KEY_F8,SLK_KEY_F9,SLK_KEY_F10,SLK_KEY_F11,SLK_KEY_F12,
   SLK_KEY_UP,SLK_KEY_DOWN,SLK_KEY_LEFT,SLK_KEY_RIGHT,
   SLK_KEY_SPACE,SLK_KEY_TAB,SLK_KEY_SHIFT,SLK_KEY_CTRL,SLK_KEY_INS,
   SLK_KEY_DEL,SLK_KEY_HOME,SLK_KEY_END,SLK_KEY_PGUP,SLK_KEY_PGDN,
   SLK_KEY_BACK,SLK_KEY_ESCAPE,SLK_KEY_RETURN,SLK_KEY_ENTER,SLK_KEY_PAUSE,SLK_KEY_SCROLL,
   SLK_KEY_NP0,SLK_KEY_NP1,SLK_KEY_NP2,SLK_KEY_NP3,SLK_KEY_NP4,SLK_KEY_NP5,SLK_KEY_NP6,SLK_KEY_NP7,SLK_KEY_NP8,SLK_KEY_NP9,
   SLK_KEY_NP_MUL,SLK_KEY_NP_DIV,SLK_KEY_NP_ADD,SLK_KEY_NP_SUB,SLK_KEY_NP_DECIMAL,
};

enum SLK_gamepad_button
{
   SLK_PAD_A, SLK_PAD_B, SLK_PAD_X, SLK_PAD_Y, SLK_PAD_BACK, SLK_PAD_GUIDE, SLK_PAD_MAX,
   SLK_PAD_START, SLK_PAD_LEFTSTICK, SLK_PAD_RIGHTSTICK, SLK_PAD_LEFTSHOULDER,
   SLK_PAD_RIGHTSHOULDER, SLK_PAD_UP, SLK_PAD_DOWN, SLK_PAD_LEFT, SLK_PAD_RIGHT,
};

enum SLK_mouse_button
{
   SLK_BUTTON_LEFT,SLK_BUTTON_RIGHT,SLK_BUTTON_MIDDLE,SLK_BUTTON_X1,SLK_BUTTON_X2,
};

enum SLK_layer
{
   SLK_LAYER_PAL,SLK_LAYER_RGB,
};

enum SLK_mask
{
  SLK_OPAQUE = 0,
  SLK_TRANSPARENT = 255,
};

enum SLK_flip
{
   SLK_FLIP_NONE = 0,
   SLK_FLIP_VERTICAL = 1,
   SLK_FLIP_HORIZONTAL = 2,
};

enum SLK_window
{
   SLK_WINDOW_MAX = -1,
};

#endif //_SLK_TYPES_H_
