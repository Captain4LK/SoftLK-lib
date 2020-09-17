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
   SLK_Pal_sprite *s = malloc(sizeof(SLK_Pal_sprite));

   s->width = width;
   s->height = height;

   s->data = malloc(width*height*sizeof(SLK_Paxel));
   memset(s->data,0,sizeof(SLK_Paxel)*width*height);

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

//Returns the paxel at the specified
//location.
//The paxel layout may change in
//the future.
SLK_Paxel SLK_pal_sprite_get_paxel(const SLK_Pal_sprite *s, int x, int y)
{
   if(INBOUNDS(0,s->width,x)&&INBOUNDS(0,s->height,y))
      return s->data[y*s->width+x];
   else
      return SLK_color_create_paxel(0,0);
}

//Sets the paxel at the specified
//position.
//Again, the paxel layout may change
//in the future.
void SLK_pal_sprite_set_paxel(SLK_Pal_sprite *s, int x, int y, SLK_Paxel c)
{
   if(INBOUNDS(0,s->width,x)&&INBOUNDS(0,s->height,y))
      s->data[y*s->width+x] = c;
}

//Loads a sprite from a .slk file.
//Current layout:
//int width,
//int height,
//width*height*SLK_Paxel data
SLK_Pal_sprite *SLK_pal_sprite_load(const char *path)
{
   FILE *f = fopen(path,"rb");
   SLK_Pal_sprite *s = NULL;
   int width, height;
   char file_type[512];

   if(f==NULL)
   {
      printf("Failed to open %s!\n",path);

      return SLK_pal_sprite_create(1,1);
   }

   fread(file_type,sizeof(char),8,f);
   file_type[8] = '\0';
   if(strcmp(file_type,"SLKIMAGE")!=0)
   {
      printf("%s does not seem to be a SLKIMAGE file\n",path);
      return SLK_pal_sprite_create(1,1);
   }
      
   fread(&width,sizeof(int),1,f);
   fread(&height,sizeof(int),1,f);   
   
   s = SLK_pal_sprite_create(width,height);
   fread(s->data,sizeof(SLK_Paxel),width*height,f);
   fclose(f);
   
   return s;
}

//Saves a sprite to a file
//with the above specified
//layout.
void SLK_pal_sprite_save(const char *path, const SLK_Pal_sprite *s)
{
   FILE *f = fopen(path,"wb");

   if(!f)
      return;
      
   fwrite(&s->width,sizeof(int),1,f);
   fwrite(&s->height,sizeof(int),1,f);
   fwrite(s->data,sizeof(SLK_Paxel),s->width*s->height,f);
   fclose(f);
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
         SLK_Paxel c = SLK_pal_sprite_get_paxel(src, tx + ox, ty + oy);
         SLK_pal_sprite_set_paxel(dst,x + tx, y +ty, c);
      }
   }
}

//Copies the data o a sprite to another one.
//Usefull for duplicating sprites.
void SLK_pal_sprite_copy(SLK_Pal_sprite *dst, const SLK_Pal_sprite *src)
{
   for(int x = 0;x<src->width;x++)
      for(int y = 0;y<src->height;y++)
         SLK_pal_sprite_set_paxel(dst,x,y,SLK_pal_sprite_get_paxel(src,x,y));
}
//-------------------------------------