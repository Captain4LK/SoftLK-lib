/*
(C) 2019 Lukas Holzbeierlein (Captain4LK)

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
