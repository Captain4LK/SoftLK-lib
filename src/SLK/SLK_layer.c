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
       * Neither the name of SoftLK nor the
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
#include "SLK_variables.h"

void SLK_layer_create(const unsigned index, const int type)
{
   if(index>=layer_count)
      return;

   layers[index].type = type;
   layers[index].active = 1;
   layers[index].tint = SLK_color_create(255,255,255,255);

   switch(type)
   {
   case SLK_LAYER_PAL:
      layers[index].type_0.target = SLK_pal_sprite_create(screen_width,screen_height);
      layers[index].type_0.render = SLK_rgb_sprite_create(screen_width,screen_height);

      glGenTextures(1,&layers[index].type_0.texture);
      glBindTexture(GL_TEXTURE_2D,layers[index].type_0.texture);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
      glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
      glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,screen_width,screen_height,
                   0,GL_RGBA,GL_UNSIGNED_BYTE,layers[index].type_0.render->data);

      break;
   case SLK_LAYER_RGB:
      layers[index].type_1.target = SLK_rgb_sprite_create(screen_width,screen_height);

      glGenTextures(1,&layers[index].type_1.texture);
      glBindTexture(GL_TEXTURE_2D,layers[index].type_1.texture);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
      glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
      glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,screen_width,screen_height,
                   0,GL_RGBA,GL_UNSIGNED_BYTE,layers[index].type_1.target->data);


      break;
   case SLK_LAYER_GPU:

      break;
   }
}

void SLK_layer_activate(const unsigned index, const int active)
{
   if(index>=layer_count)
      return;

   layers[index].active = active;
}

void SLK_layer_set_palette(const unsigned index, SLK_Palette *pal)
{
   if(index>=layer_count||layers[index].type!=SLK_LAYER_PAL)
      return;

   layers[index].type_0.palette = pal;
}

void SLK_layer_set_tint(unsigned index, SLK_Color tint)
{
   layers[index].tint = tint;
}

void SLK_layer_set_current(const unsigned index)
{
   if(index>=layer_count)
      return;

   current_layer = &layers[index];

   if(layers[index].type==SLK_LAYER_PAL)
   {
      target_pal_default = layers[index].type_0.target;
      target_pal = layers[index].type_0.target;
   }
   else if(layers[index].type==SLK_LAYER_RGB)
   {
      target_rgb_default = layers[index].type_1.target;
      target_rgb = layers[index].type_1.target;
   }
}
