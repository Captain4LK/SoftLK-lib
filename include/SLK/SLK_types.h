/*
(C) 2020 Lukas Holzbeierlein (Captain4LK)

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

#ifndef _SLK_TYPES_H_

#define _SLK_TYPES_H_

#include <stdint.h>
#include "../glad/glad.h"

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
   GLint id;
}SLK_GPU_sprite;

typedef struct
{
   SLK_Pal_sprite *target;
   SLK_RGB_sprite *render;
   SLK_Palette *palette;
   GLuint texture;
}SLK_Layer_pal;

typedef struct
{
   SLK_RGB_sprite *target;
   GLuint texture;
}SLK_Layer_rgb;

typedef struct
{
   
}SLK_Layer_gpu;

typedef struct
{
   int type;
   int active;
   SLK_Color tint;

   union
   {
      SLK_Layer_pal type_0;
      SLK_Layer_rgb type_1;
      SLK_Layer_gpu type_2;
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
   SLK_KEY_K0,SLK_KEY_K1,SLK_KEY_K2,SLK_KEY_K3,SLK_KEY_K4,SLK_KEY_K5,
   SLK_KEY_K6,SLK_KEY_K7,SLK_KEY_K8,SLK_KEY_K9,
   SLK_KEY_F1,SLK_KEY_F2,SLK_KEY_F3,SLK_KEY_F4,SLK_KEY_F5,SLK_KEY_F6,
   SLK_KEY_F7,SLK_KEY_F8,SLK_KEY_F9,SLK_KEY_F10,SLK_KEY_F11,SLK_KEY_F12,
   SLK_KEY_UP,SLK_KEY_DOWN,SLK_KEY_LEFT,SLK_KEY_RIGHT,
   SLK_KEY_SPACE,SLK_KEY_TAB,SLK_KEY_SHIFT,SLK_KEY_CTRL,SLK_KEY_INS,
   SLK_KEY_DEL,SLK_KEY_HOME,SLK_KEY_END,SLK_KEY_PGUP,SLK_KEY_PGDN,
   SLK_KEY_BACK,SLK_KEY_ESCAPE,SLK_KEY_RETURN,SLK_KEY_ENTER,SLK_KEY_PAUSE,SLK_KEY_SCROLL,
   SLK_KEY_NP0,SLK_KEY_NP1,SLK_KEY_NP2,SLK_KEY_NP3,SLK_KEY_NP4,SLK_KEY_NP5,SLK_KEY_NP6,SLK_KEY_NP7,SLK_KEY_NP8,SLK_KEY_NP9,
   SLK_KEY_NP_MUL,SLK_KEY_NP_DIV,SLK_KEY_NP_ADD,SLK_KEY_NP_SUB,SLK_KEY_NP_DECIMAL,
};

enum SLK_mouse_button
{
   SLK_BUTTON_LEFT,SLK_BUTTON_RIGHT,SLK_BUTTON_MIDDLE,SLK_BUTTON_X1,SLK_BUTTON_X2,
};

enum SLK_layer
{
   SLK_LAYER_PAL,SLK_LAYER_RGB,SLK_LAYER_GPU,
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

enum SLK_window_scale
{
   SLK_WINDOW_MAX = -1,
};

#endif //_SLK_TYPES_H_
