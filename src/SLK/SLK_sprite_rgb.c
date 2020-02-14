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
