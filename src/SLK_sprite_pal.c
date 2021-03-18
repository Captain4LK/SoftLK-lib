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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//-------------------------------------

//Internal includes
#include "../include/SLK/SLK_types.h"
#include "../include/SLK/SLK_functions.h"
#include "backend.h"
//-------------------------------------

//#defines
#define INBOUNDS(LOWER,UPPER,NUMBER) \
            ((unsigned)(NUMBER-LOWER)<(UPPER-LOWER))
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
//-------------------------------------

//Function prototypes
//-------------------------------------

//Function implementations

//Creates a sprite with the requested dimensions
//and returns a pointer to its location.
SLK_Pal_sprite *SLK_pal_sprite_create(int width, int height)
{
   SLK_Pal_sprite *s = malloc(sizeof(*s));

   s->width = width;
   s->height = height;

   s->data = malloc(width*height*sizeof(*s->data));
   memset(s->data,0,sizeof(*s->data)*width*height);

   return s;
}

//Destroys a previously allocated sprite.
//The user may not now that s->data also 
//needs to be freed.
void SLK_pal_sprite_destroy(SLK_Pal_sprite *s)
{
   if(s==NULL)
      return;

   free(s->data);
   free(s);
}

//Returns the index at the specified
//location.
uint8_t SLK_pal_sprite_get_index(const SLK_Pal_sprite *s, int x, int y)
{
   if(INBOUNDS(0,s->width,x)&&INBOUNDS(0,s->height,y))
      return s->data[y*s->width+x];
   else
      return 0;
}

//Sets the index at the specified
//position.
void SLK_pal_sprite_set_index(SLK_Pal_sprite *s, int x, int y, uint8_t c)
{
   if(INBOUNDS(0,s->width,x)&&INBOUNDS(0,s->height,y))
      s->data[y*s->width+x] = c;
}

//Loads a sprite from a .slk file.
SLK_Pal_sprite *SLK_pal_sprite_load(const char *path)
{
   return backend_load_pal(path);
}

//Loads a sprite from a .slk file.
SLK_Pal_sprite *SLK_pal_sprite_load_file(FILE *f)
{
   return backend_load_pal_file(f);
}

//Loads a sprite from a .slk file.
SLK_Pal_sprite *SLK_pal_sprite_load_mem(const void *data, int length)
{
   return backend_load_pal_mem(data,length);
}

//Writes a sprite to a file
//with the above specified
//layout.
//rle modes:
//0: No RLE
//1: RLE for indices
void SLK_pal_sprite_save(const char *path, const SLK_Pal_sprite *s, int rle)
{
   backend_save_pal(s,path,rle);
}

//Writes a sprite to a file stream
//with the above specified
//layout.
//rle modes:
//0: No RLE
//1: RLE for indices
void SLK_pal_sprite_save_file(FILE *f, const SLK_Pal_sprite *s, int rle)
{
   backend_save_pal_file(s,f,rle);
}

//Copies a specified part of the data of a sprite 
//to another one.
//Usefull for splitting a texture atlas in individual sprites.
void SLK_pal_sprite_copy_partial(SLK_Pal_sprite *dst, const SLK_Pal_sprite *src, int x, int y, int ox, int oy, int width, int height)
{
   for(int tx = 0; tx < width; tx++)
   {
      for(int ty = 0; ty < height; ty++)
      {
         uint8_t c = SLK_pal_sprite_get_index(src, tx + ox, ty + oy);
         SLK_pal_sprite_set_index(dst,x + tx, y +ty, c);
      }
   }
}

//Copies the data from a sprite to another one.
//Usefull for duplicating sprites.
void SLK_pal_sprite_copy(SLK_Pal_sprite *dst, const SLK_Pal_sprite *src)
{
   for(int x = 0;x<src->width;x++)
      for(int y = 0;y<src->height;y++)
         SLK_pal_sprite_set_index(dst,x,y,SLK_pal_sprite_get_index(src,x,y));
}
//-------------------------------------
