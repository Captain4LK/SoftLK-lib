/*
   Copyright (c) 2020, Lukas Holzbeierlein (Captain4LK) 
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
       * Redistributions of source code must retain the above copyright
         notice, this list of conditions and the following disclaimer.
       * Redistributions in binary form must reproduce the above copyright
         notice, this list of conditions and the following disclaimer in the
         documentation and/or other materials provided with the distribution.
       * Neither the name of SLK nor the
         names of its contributors may be used to endorse or promote products
         derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL CAPTAIN4LK BE LIABLE FOR ANY
   DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
   ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "../../include/SLK/SLK.h"
#include "../../include/SLK/SLK_functions.h"
#include "SLK_variables.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION 
#include "stb_image_write.h"
//https://github.com/nothings/stb

SLK_RGB_sprite *SLK_rgb_sprite_create(const int width, const int height)
{   
   SLK_RGB_sprite *s = malloc(sizeof(SLK_RGB_sprite));
   
   s->width = width;
   s->height = height;
   
   s->data = malloc(width*height*sizeof(SLK_Color));
   memset(s->data,0,width*height*sizeof(SLK_Color));
    
   return s;
}

void SLK_rgb_sprite_destroy(SLK_RGB_sprite *s)
{
   free(s->data);
   free(s);
}

SLK_Color SLK_rgb_sprite_get_pixel(const SLK_RGB_sprite *s, const int x, const int y)
{
   if(INBOUNDS(0,s->width,x)&&INBOUNDS(0,s->height,y))
      return s->data[y*s->width+x];
   else
      return SLK_color_create(0,0,0,0);
}

void SLK_rgb_sprite_set_pixel(SLK_RGB_sprite *s, const int x, const int y, const SLK_Color c)
{
   if(INBOUNDS(0,s->width,x)&&INBOUNDS(0,s->height,y))
      s->data[y*s->width+x] = c;
}

SLK_RGB_sprite *SLK_rgb_sprite_load(const char *path)
{
   unsigned char *data = NULL;
   int width = 1;
   int height = 1;
   SLK_RGB_sprite *out;

   data = stbi_load(path,&width,&height,NULL,4);
   if(data==NULL)
   {
      printf("Unable to load %s\n",path);
      return SLK_rgb_sprite_create(1,1);
   }

   out = SLK_rgb_sprite_create(width,height);
   memcpy(out->data,data,width*height*sizeof(SLK_Color));

   stbi_image_free(data);

   return out;
}

void SLK_rgb_sprite_save(const char *path, const SLK_RGB_sprite *s)
{
   stbi_write_png(path,s->width,s->height,4,(void *)s->data,s->width*sizeof(SLK_Color));
}
