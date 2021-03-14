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
//-------------------------------------

//Internal includes
#include "../include/SLK/SLK_types.h"
#include "../include/SLK/SLK_functions.h"
#include "backend.h"
#include "SLK_draw_rgb_i.h"
#include "SLK_draw_pal_i.h"
#include "SLK_layer_i.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
static uint32_t rgb332_int[256] = 
{
   0xFF000000, 0xFF000020, 0xFF000040, 0xFF000060, 0xFF000080, 0xFF0000A0, 0xFF0000C0, 0xFF0000E0, 0xFF002000, 0xFF002020, 0xFF002040, 0xFF002060, 0xFF002080, 0xFF0020A0, 0xFF0020C0, 0xFF0020E0,
   0xFF004000, 0xFF004020, 0xFF004040, 0xFF004060, 0xFF004080, 0xFF0040A0, 0xFF0040C0, 0xFF0040E0, 0xFF006000, 0xFF006020, 0xFF006040, 0xFF006060, 0xFF006080, 0xFF0060A0, 0xFF0060C0, 0xFF0060E0,
   0xFF008000, 0xFF008020, 0xFF008040, 0xFF008060, 0xFF008080, 0xFF0080A0, 0xFF0080C0, 0xFF0080E0, 0xFF00A000, 0xFF00A020, 0xFF00A040, 0xFF00A060, 0xFF00A080, 0xFF00A0A0, 0xFF00A0C0, 0xFF00A0E0,
   0xFF00C000, 0xFF00C020, 0xFF00C040, 0xFF00C060, 0xFF00C080, 0xFF00C0A0, 0xFF00C0C0, 0xFF00C0E0, 0xFF00E000, 0xFF00E020, 0xFF00E040, 0xFF00E060, 0xFF00E080, 0xFF00E0A0, 0xFF00E0C0, 0xFF00E0E0,
   0xFF400000, 0xFF400020, 0xFF400040, 0xFF400060, 0xFF400080, 0xFF4000A0, 0xFF4000C0, 0xFF4000E0, 0xFF402000, 0xFF402020, 0xFF402040, 0xFF402060, 0xFF402080, 0xFF4020A0, 0xFF4020C0, 0xFF4020E0,
   0xFF404000, 0xFF404020, 0xFF404040, 0xFF404060, 0xFF404080, 0xFF4040A0, 0xFF4040C0, 0xFF4040E0, 0xFF406000, 0xFF406020, 0xFF406040, 0xFF406060, 0xFF406080, 0xFF4060A0, 0xFF4060C0, 0xFF4060E0,
   0xFF408000, 0xFF408020, 0xFF408040, 0xFF408060, 0xFF408080, 0xFF4080A0, 0xFF4080C0, 0xFF4080E0, 0xFF40A000, 0xFF40A020, 0xFF40A040, 0xFF40A060, 0xFF40A080, 0xFF40A0A0, 0xFF40A0C0, 0xFF40A0E0,
   0xFF40C000, 0xFF40C020, 0xFF40C040, 0xFF40C060, 0xFF40C080, 0xFF40C0A0, 0xFF40C0C0, 0xFF40C0E0, 0xFF40E000, 0xFF40E020, 0xFF40E040, 0xFF40E060, 0xFF40E080, 0xFF40E0A0, 0xFF40E0C0, 0xFF40E0E0,
   0xFF800000, 0xFF800020, 0xFF800040, 0xFF800060, 0xFF800080, 0xFF8000A0, 0xFF8000C0, 0xFF8000E0, 0xFF802000, 0xFF802020, 0xFF802040, 0xFF802060, 0xFF802080, 0xFF8020A0, 0xFF8020C0, 0xFF8020E0, 
   0xFF804000, 0xFF804020, 0xFF804040, 0xFF804060, 0xFF804080, 0xFF8040A0, 0xFF8040C0, 0xFF8040E0, 0xFF806000, 0xFF806020, 0xFF806040, 0xFF806060, 0xFF806080, 0xFF8060A0, 0xFF8060C0, 0xFF8060E0, 
   0xFF808000, 0xFF808020, 0xFF808040, 0xFF808060, 0xFF808080, 0xFF8080A0, 0xFF8080C0, 0xFF8080E0, 0xFF80A000, 0xFF80A020, 0xFF80A040, 0xFF80A060, 0xFF80A080, 0xFF80A0A0, 0xFF80A0C0, 0xFF80A0E0,
   0xFF80C000, 0xFF80C020, 0xFF80C040, 0xFF80C060, 0xFF80C080, 0xFF80C0A0, 0xFF80C0C0, 0xFF80C0E0, 0xFF80E000, 0xFF80E020, 0xFF80E040, 0xFF80E060, 0xFF80E080, 0xFF80E0A0, 0xFF80E0C0, 0xFF80E0E0,
   0xFFC00000, 0xFFC00020, 0xFFC00040, 0xFFC00060, 0xFFC00080, 0xFFC000A0, 0xFFC000C0, 0xFFC000E0, 0xFFC02000, 0xFFC02020, 0xFFC02040, 0xFFC02060, 0xFFC02080, 0xFFC020A0, 0xFFC020C0, 0xFFC020E0,
   0xFFC04000, 0xFFC04020, 0xFFC04040, 0xFFC04060, 0xFFC04080, 0xFFC040A0, 0xFFC040C0, 0xFFC040E0, 0xFFC06000, 0xFFC06020, 0xFFC06040, 0xFFC06060, 0xFFC06080, 0xFFC060A0, 0xFFC060C0, 0xFFC060E0, 
   0xFFC08000, 0xFFC08020, 0xFFC08040, 0xFFC08060, 0xFFC08080, 0xFFC080A0, 0xFFC080C0, 0xFFC080E0, 0xFFC0A000, 0xFFC0A020, 0xFFC0A040, 0xFFC0A060, 0xFFC0A080, 0xFFC0A0A0, 0xFFC0A0C0, 0xFFC0A0E0, 
   0xFFC0C000, 0xFFC0C020, 0xFFC0C040, 0xFFC0C060, 0xFFC0C080, 0xFFC0C0A0, 0xFFC0C0C0, 0xFFC0C0E0, 0xFFC0E000, 0xFFC0E020, 0xFFC0E040, 0xFFC0E060, 0xFFC0E080, 0xFFC0E0A0, 0xFFC0E0C0, 0xFFC0E0E0,
};
static SLK_Palette *rgb332 = (SLK_Palette*)rgb332_int;

int dynamic;
int layer_count;
SLK_Layer *layers;
SLK_Layer *current_layer;
//-------------------------------------

//Function prototypes
//-------------------------------------

//Function implementations

//Initializes the requested layer to the specified type.
//These types are currently availible:
//SLK_LAYER_RGB,
//SLK_LAYER_PAL
void SLK_layer_create(unsigned index, SLK_layer type)
{
   if(index>=layer_count)
      return;

   layers[index].type = type;
   layers[index].active = 1;
   layers[index].tint = SLK_color_create(255,255,255,255);
   layers[index].x = 0;
   layers[index].y = 0;
   layers[index].scale = 1.0f;
   int screen_width = backend_get_width();
   int screen_height = backend_get_height();

   switch(type)
   {
   case SLK_LAYER_PAL:
      {
         layers[index].type_0.target = SLK_pal_sprite_create(screen_width,screen_height);
         layers[index].type_0.render = SLK_rgb_sprite_create(screen_width,screen_height);
         backend_create_layer(index,type);
         layers[index].type_0.palette = rgb332;
      }
      break;
   case SLK_LAYER_RGB:
      {
         layers[index].type_1.target = SLK_rgb_sprite_create(screen_width,screen_height);
         backend_create_layer(index,type);
      }
      break;
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

   if(pal==NULL)
      layers[index].type_0.palette = rgb332;
   else
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
