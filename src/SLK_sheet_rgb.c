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

//Creates a sprite sheet with the specified storage size and
//returns a pointer to it.
SLK_RGB_sheet *SLK_rgb_sheet_create(int size)
{
   SLK_RGB_sheet *sheet = malloc(sizeof(SLK_RGB_sheet));
   memset(sheet,0,sizeof(SLK_RGB_sheet));
   sheet->size = size;
   sheet->sheet = malloc(sizeof(SLK_RGB_sprite *)*size);

   return sheet;
}

//Expands (or shrinks) the size of a sprite sheet.
void SLK_rgb_sheet_expand(SLK_RGB_sheet *sheet, int new_size)
{
   int old_size = sheet->size;
   if(old_size<new_size)
   {
      for(int i = new_size;i<sheet->size;i++)
         SLK_rgb_sprite_destroy(sheet->sheet[i]);
   }
   sheet->size = new_size;
   sheet->sheet = realloc(sheet->sheet,sizeof(SLK_RGB_sprite *)*new_size);
   if(old_size<new_size)
      memset(sheet->sheet+old_size,0,sizeof(SLK_RGB_sprite *)*(new_size-old_size));
}

//Destroys a sprite sheet.
void SLK_rgb_sheet_destroy(SLK_RGB_sheet *sheet)
{
   for(int i  = 0;i<sheet->size;i++)
      SLK_rgb_sprite_destroy(sheet->sheet[i]);
   free(sheet->sheet);
   free(sheet);
}

//Copies a sprite to the specified 
//sprite sheet and index.
void SLK_rgb_sheet_add(SLK_RGB_sheet *sheet, int index, SLK_RGB_sprite *sprite)
{
   if(index<0||index>sheet->size-1)
   {
      printf("Error sprite sheet index out of bounds\n");
      return;
   }
   if(sheet->sheet[index]!=NULL)
      SLK_rgb_sprite_destroy(sheet->sheet[index]);

   sheet->sheet[index] = SLK_rgb_sprite_create(sprite->width,sprite->height);
   SLK_rgb_sprite_copy(sheet->sheet[index],sprite);
}

//Copies a sprite from a specified image part
//to the sprite sheet.
void SLK_rgb_seet_add_from(SLK_RGB_sheet *sheet, int index, SLK_RGB_sprite *source, int x, int y, int width, int height)
{
   if(index<0||index>sheet->size-1)
   {
      printf("Error sprite sheet index out of bounds\n");
      return;
   }
   if(source==NULL)
   {
      printf("Error: no source specified\n");
      return;
   }
   if(sheet->sheet[index]!=NULL)
      SLK_rgb_sprite_destroy(sheet->sheet[index]);

   sheet->sheet[index] = SLK_rgb_sprite_create(width,height);
   SLK_rgb_sprite_copy_partial(sheet->sheet[index],source,0,0,x,y,width,height);
}

//Returns the sprite at the specified sprite sheet index.
SLK_RGB_sprite *SLK_rgb_sheet_get(const SLK_RGB_sheet *sheet, int index)
{
   if(index<0||index>sheet->size-1)
   {
      printf("Error sprite sheet index out of bounds\n");
      return NULL;
   }

   return sheet->sheet[index];
}
//-------------------------------------
