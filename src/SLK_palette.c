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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//-------------------------------------

//Internal includes
#include "../include/SLK/SLK_types.h"
#include "../include/SLK/SLK_functions.h"
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

   FILE *f = fopen(path,"r");
   if(!f)
   {
      printf("Unable to load palette\n");
      return NULL;
   }

   SLK_Palette *palette = malloc(sizeof(*palette));
   memset(palette,0,sizeof(*palette));
   for(i = 0;i<259&&fgets(buffer,512,f);i++)
   {
      if(i==2)
      {
         sscanf(buffer,"%d",&found);
      }
      else if(i>2&&buffer[0]!='\0')
      {
         if(sscanf(buffer,"%d %d %d %d",&r,&g,&b,&a)!=4)
         {
            sscanf(buffer,"%d %d %d",&r,&g,&b);
            a = 255;
         }

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
