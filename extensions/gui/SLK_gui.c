/* 
LICENSE OPTION A: 3-clause BSD

   Copyright (C) 2021 Captain4LK (Lukas Holzbeierlein)

   Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

   3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

LICENSE OPTION B: Public Domain - Unlicense

   This is free and unencumbered software released into the public domain.

   Anyone is free to copy, modify, publish, use, compile, sell, or
   distribute this software, either in source code form or as a compiled
   binary, for any purpose, commercial or non-commercial, and by any
   means.

   In jurisdictions that recognize copyright laws, the author or authors
   of this software dedicate any and all copyright interest in the
   software to the public domain. We make this dedication for the benefit
   of the public at large and to the detriment of our heirs and
   successors. We intend this dedication to be an overt act of
   relinquishment in perpetuity of all present and future rights to this
   software under copyright law.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
   OTHER DEALINGS IN THE SOFTWARE.

   For more information, please refer to <http://unlicense.org/>
*/

//External includes
#include <stdlib.h>
#include <string.h>
//-------------------------------------

//Internal includes
#include "../../include/SLK/SLK_types.h"
#include "../../include/SLK/SLK_functions.h"
#include "../../include/SLK/SLK_gui.h"
#include "SLK_gui_shared.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
SLK_RGB_sprite *slk_gui_font = NULL;
SLK_Color slk_gui_color_0;
SLK_Color slk_gui_color_1;
SLK_Color slk_gui_color_2;
SLK_Color slk_gui_color_3;
SLK_Color slk_gui_color_4;
SLK_Color slk_gui_color_5;
//-------------------------------------

//Function prototypes
//-------------------------------------

//Function implementations

void SLK_gui_set_font(SLK_RGB_sprite *f)
{
   slk_gui_font = f;
}

SLK_RGB_sprite *SLK_gui_get_font()
{
   return slk_gui_font;
}

void SLK_gui_set_colors(SLK_Color c0, SLK_Color c1, SLK_Color c2, SLK_Color c3, SLK_Color c4)
{
   slk_gui_color_0 = c0;
   slk_gui_color_1 = c1;
   slk_gui_color_2 = c2;
   slk_gui_color_3 = c3;
   slk_gui_color_4 = c4;
}

void SLK_gui_set_font_color(SLK_Color color)
{
   slk_gui_color_5 = color;
} 

//Internal
int clip_text(char *dst, const char *src, int dst_size, SLK_gui_rectangle area)
{
   int text_max_x = area.w/8;
   int text_max_y = area.h/8;
   int c = 0;
   int ci = 0;
   int x = 0;
   int y = 0;

   while(y<text_max_y)
   {
      x = 0;
      while(x<text_max_x)
      {
         if(src[ci]!='\n'&&src[ci]!='\0')
            dst[c] = src[ci];
         else if(src[ci]=='\n')
         {
            ci++;
            break;
         }
         else
            break;
         x++;
         c++;
         ci++;
      }
      dst[c] = '\n';
      c++;
      y++;
   }
   dst[c] = '\0';

   if(text_max_y>1) //Align left
      return 0;

   //Algin center (only applies to first line)
   int text_x = strlen(dst)-1;

   return (area.w-text_x*8)/2;
}
//-------------------------------------
