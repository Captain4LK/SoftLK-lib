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

//External includes
#include <stdio.h>
//-------------------------------------

//Internal includes
#include "../../include/SLK/SLK_types.h"
#include "../../include/SLK/SLK_functions.h"
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

//Reads a palette from a .pal file.
//Most pal files don't have an alpha
//component added to them, you
//need to add that yourself in the pal file.
SLK_Palette *SLK_palette_load(const char *path)
{
   char buffer[512];
   int colors = 0,i,found;
   int r,g,b,a;
   SLK_Palette *palette = malloc(sizeof(SLK_Palette));

   memset(palette,0,sizeof(SLK_Palette));

   FILE *f = fopen(path,"r");
   if(!f)
   {
      printf("Unable to load palette\n");
      return palette;
   }
   
   
   for(i = 0;i<259&&fgets(buffer,512,f);i++)
   {
      if(i==2)
      {
         sscanf(buffer,"%d",&found);
         printf("Found %d colors\n",found);
      }
      else if(i>2&&buffer[0]!='\0')
      {
         sscanf(buffer,"%d %d %d %d",&r,&g,&b,&a);

         palette->colors[colors].r = r;
         palette->colors[colors].g = g;
         palette->colors[colors].b = b;
         palette->colors[colors].a = a;
         colors++;
      }
   }

   return palette;
}

//Sets the color of a palette at the
//desired index.
//Usefull for rapidly chaning certain colors of a sprite,
//eg. for simple animations.
void SLK_palette_set_color(SLK_Palette *palette, int index, SLK_Color color)
{
   if(index>=0&&index<256)
      palette->colors[index] = color;
}

//Returns the color of a palette at the
//desired index.
SLK_Color SLK_palette_get_color(const SLK_Palette *palette, int index)
{
   if(index>=0&&index<256)
      return palette->colors[index];
   else
      return SLK_color_create(0,0,0,255);
}
//-------------------------------------
