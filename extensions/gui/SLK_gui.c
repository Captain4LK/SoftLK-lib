/*
SLK_gui - gui extension for SoftLK-lib

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

This work's goal is to never be encumbered by any exclusive intellectual
property rights. The work is therefore provided under CC0 1.0 + additional
WAIVER OF ALL INTELLECTUAL PROPERTY RIGHTS that waives the rest of
intellectual property rights not already waived by CC0 1.0.
The CC0 license text and ip waiver can be found in the LICENSE file.
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
