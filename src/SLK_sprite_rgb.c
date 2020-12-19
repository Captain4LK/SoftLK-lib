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

//Creates a sprites with the specified dimensions and clear it.
SLK_RGB_sprite *SLK_rgb_sprite_create(int width, int height)
{   
   SLK_RGB_sprite *s = malloc(sizeof(*s));
   
   s->width = width;
   s->height = height;
   
   s->data = malloc(width*height*sizeof(*s->data));
   memset(s->data,0,width*height*sizeof(*s->data));
    
   return s;
}

//Destroys a previously allocated sprite.
//The user may not know that s->data also needs
//to be freed.
void SLK_rgb_sprite_destroy(SLK_RGB_sprite *s)
{
   if(s==NULL)
      return;

   free(s->data);
   free(s);
}

//Returns the color at the specified position of a sprite.
SLK_Color SLK_rgb_sprite_get_pixel(const SLK_RGB_sprite *s, int x, int y)
{
   if(INBOUNDS(0,s->width,x)&&INBOUNDS(0,s->height,y))
      return s->data[y*s->width+x];
   else
      return SLK_color_create(0,0,0,0);
}

//Sets the color of a sprite at the specified position.
//Similar to SLK_draw_rgb_color, but ignores alpha value.
void SLK_rgb_sprite_set_pixel(SLK_RGB_sprite *s, int x, int y, SLK_Color c)
{
   if(INBOUNDS(0,s->width,x)&&INBOUNDS(0,s->height,y))
      s->data[y*s->width+x] = c;
}

//Loads a sprite from a png file.
SLK_RGB_sprite *SLK_rgb_sprite_load(const char *path)
{
   return backend_load_rgb(path);
}

//Loads a sprite from a png file.
SLK_RGB_sprite *SLK_rgb_sprite_load_file(FILE *f)
{
   return backend_load_rgb_file(f);
}

//Loads a sprite from a png file.
SLK_RGB_sprite *SLK_rgb_sprite_load_mem(const void *data, int length)
{
   return backend_load_rgb_mem(data, length);
}

///Saves a sprite to an image file.
//All formats that are supported by stb_image_write are
//supported by this function (png etc.).
void SLK_rgb_sprite_save(const char *path, const SLK_RGB_sprite *s)
{
   backend_save_rgb(s,path);
}

///Saves a sprite to an image file.
//All formats that are supported by stb_image_write are
//supported by this function (png etc.).
void SLK_rgb_sprite_save_file(FILE *f, const SLK_RGB_sprite *s)
{
   backend_save_rgb_file(s,f);
}

//Copies the data of a sprite to another one.
//Usefull for duplicating sprites.
void SLK_rgb_sprite_copy(SLK_RGB_sprite *dst, const SLK_RGB_sprite *src)
{
   for(int x = 0;x<src->width;x++)
      for(int y = 0;y<src->height;y++)
         SLK_rgb_sprite_set_pixel(dst,x,y,SLK_rgb_sprite_get_pixel(src,x,y));
}

//Copies a specified part of a sprite to another one.
//Usefull for creating sprite sheets.
void SLK_rgb_sprite_copy_partial(SLK_RGB_sprite *dst, const SLK_RGB_sprite *src, int x, int y, int ox, int oy, int width, int height)
{
   for(int tx = 0;tx<width;tx++)
      for(int ty = 0;ty<height;ty++)
         SLK_rgb_sprite_set_pixel(dst,x+tx,y+ty,SLK_rgb_sprite_get_pixel(src,ox+tx,oy+ty));
}
//-------------------------------------
