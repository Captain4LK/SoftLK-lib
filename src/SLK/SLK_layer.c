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
#include <SDL2/SDL.h>
#include "../../include/glad/glad.h"
//-------------------------------------

//Internal includes
#include "../../include/SLK/SLK_types.h"
#include "../../include/SLK/SLK_functions.h"
#include "SLK_render_i.h"
#include "SLK_draw_rgb_i.h"
#include "SLK_draw_pal_i.h"
#include "SLK_layer_i.h"
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

//Initializes the requested layer to the specified type.
//These types are currently availible:
//SLK_LAYER_RGB,
//SLK_LAYER_PAL
void SLK_layer_create(unsigned index, int type)
{
   if(index>=layer_count)
      return;

   layers[index].type = type;
   layers[index].active = 1;
   layers[index].tint = SLK_color_create(255,255,255,255);
   layers[index].x = 0;
   layers[index].y = 0;
   layers[index].scale = 1.0f;

   switch(type)
   {
   case SLK_LAYER_PAL:
   {
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
   }
   case SLK_LAYER_RGB:
   {
      layers[index].type_1.target = SLK_rgb_sprite_create(screen_width,screen_height);

      glGenTextures(1,&layers[index].type_1.texture);
      glBindTexture(GL_TEXTURE_2D,layers[index].type_1.texture);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
      glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
      glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,screen_width,screen_height,
                   0,GL_RGBA,GL_UNSIGNED_BYTE,layers[index].type_1.target->data);


      break;
   }
   case SLK_LAYER_GPU:
   {

      break;
   }
   }
}

//Sets wether the layers is supposed to be drawn.
void SLK_layer_activate(unsigned index, int active)
{
   if(index>=layer_count)
      return;

   layers[index].active = active;
}

//Sets the palette of a layer.
//Only works for SLK_LAYER_PAL layer type.
void SLK_layer_set_palette(unsigned index, SLK_Palette *pal)
{
   if(index>=layer_count||layers[index].type!=SLK_LAYER_PAL)
      return;

   layers[index].type_0.palette = pal;
}

//Sets the tint a layers is supposed to be drawn with.
void SLK_layer_set_tint(unsigned index, SLK_Color tint)
{
   if(index<layer_count)
      layers[index].tint = tint;
}

//Sets wether the layers should be resized on 
//window resize.
void SLK_layer_set_dynamic(unsigned index, int dynamic)
{
   if(index<layer_count)
      layers[index].dynamic = dynamic;
}

//Sets the position the layers is supposed to be
//drawn at.
void SLK_layer_set_pos(unsigned index, int x, int y)
{
   if(index<layer_count)
   {
      layers[index].x = x;
      layers[index].y = y;
   }
}

//Sets the factor the layer is supposed to be scale
//with when drawn.
void SLK_layer_set_scale(unsigned index, float scale)
{
   if(index<layer_count)
      layers[index].scale = scale;
}

//Sets the size of a non
//dynamic layer.
void SLK_layer_set_size(unsigned index, int width, int height)
{
   if(index<layer_count)
   {
      layers[index].resized = 1;

      if(layers[index].type==SLK_LAYER_PAL)
      {
         if(layers[index].type_0.target==NULL||layers[index].type_0.render==NULL)
         {
            printf("Error: Layer %d has not been created yet!\n",index);
            return;
         }

         SLK_Pal_sprite *sprite_new = SLK_pal_sprite_create(width,height);
         
         SLK_rgb_sprite_destroy(layers[index].type_0.render);
         layers[index].type_0.render = SLK_rgb_sprite_create(width,height);

         SLK_pal_sprite_copy(sprite_new,layers[index].type_0.target);
         SLK_pal_sprite_destroy(layers[index].type_0.target);
         layers[index].type_0.target = sprite_new;
      }
      else if(layers[index].type==SLK_LAYER_RGB)
      {
         if(layers[index].type_1.target==NULL)
         {
            printf("Error: Layer %d has not been created yet!\n",index);
            return;
         }

         SLK_RGB_sprite *sprite_new = SLK_rgb_sprite_create(width,height);
         sprite_new->changed = layers[index].type_1.target->changed;

         SLK_rgb_sprite_copy(sprite_new,layers[index].type_1.target);
         SLK_rgb_sprite_destroy(layers[index].type_1.target);
         layers[index].type_1.target = sprite_new;
      }

      SLK_layer_set_current(index);
   }
}

//Stores the dimensions of the specified layer in the specified pointers.
void SLK_layer_get_size(unsigned index, int *width, int *height)
{
   if(index>=layer_count)
      return;

   if(layers[index].type==SLK_LAYER_RGB)
   {
      if(width)
         *width = layers[index].type_1.target->width;
      if(height)
         *height = layers[index].type_1.target->height;
   }
   else if(layers[index].type==SLK_LAYER_PAL)
   {
      if(width)
         *width = layers[index].type_0.target->width;
      if(height)
         *height = layers[index].type_0.target->height;
   } 
}

//Sets wich layer is the current default draw target.
//Also overwrites the current draw target.
void SLK_layer_set_current(unsigned index)
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

//Returns wether the layer has been resized.
//A layer counts as resized between a SLK_layer_set_size call and a SLK_render_update call.
int SLK_layer_get_resized(unsigned index)
{
   if(index>=layer_count)
      return 0;

   return layers[index].resized;
}

//Returns the specified layer if it
//exists.
SLK_Layer *SLK_layer_get(unsigned index)
{
   if(index>=layer_count)
      return NULL;

   return &layers[index];
}
//-------------------------------------
