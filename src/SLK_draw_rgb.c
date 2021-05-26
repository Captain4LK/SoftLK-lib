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
#include <stdlib.h>
//-------------------------------------

//Internal includes
#include "../include/SLK/SLK_config.h"
#include "../include/SLK/SLK_types.h"
#include "../include/SLK/SLK_functions.h"
#include "SLK_draw_rgb_i.h"
//-------------------------------------

//#defines
#define INBOUNDS(LOWER,UPPER,NUMBER) \
            ((unsigned)(NUMBER-LOWER)<(UPPER-LOWER))

#define SIGNUM(NUM) \
   (NUM==0?0:(NUM<0?-1:1))

#define SWAP(x,y) \
            { (x)=(x)^(y); (y)=(x)^(y); (x)=(x)^(y); }
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables

#if SLK_ENABLE_RGB

static SLK_Color target_rgb_clear;
static SLK_RGB_sprite *text_sprite_rgb;
static SLK_RGB_sprite *text_sprite_rgb_default;

#endif

SLK_RGB_sprite *target_rgb;
SLK_RGB_sprite *target_rgb_default;
//-------------------------------------

//Function prototypes
//-------------------------------------

//Function implementations

#if SLK_ENABLE_RGB

//Gets the current draw target.
//Usefull for getting the default draw target.
SLK_RGB_sprite *SLK_draw_rgb_get_target()
{
   return target_rgb;
}

//Sets the draw target for rgb drawing operations.
//Pass NULL to set to default draw target.
void SLK_draw_rgb_set_target(SLK_RGB_sprite *s)
{
   if(s==NULL)
   {
      target_rgb = target_rgb_default;
      return;
   }

   target_rgb = s;
}

//Sets the color the draw target is to be cleared to
//when calling SLK_draw_rgb_clear.
void SLK_draw_rgb_set_clear_color(SLK_Color color)
{
   target_rgb_clear = color;
}

//Sets wether the target has been changed.
//The gpu texture will only be updated if the target
//has been flagged as changed.
void SLK_draw_rgb_set_changed(int changed)
{
   target_rgb->changed = changed;
}

//Returns wether the changed
//attribute has been set on the current
//draw target.
int SLK_draw_rgb_get_changed()
{
   return target_rgb->changed;
}

//Loads a new font sprite.
//Replaces the default font sprite.
void SLK_draw_rgb_load_font(const char *path)
{
   SLK_rgb_sprite_destroy(text_sprite_rgb_default);
   text_sprite_rgb_default = SLK_rgb_sprite_load(path);
   text_sprite_rgb = text_sprite_rgb_default;
}

//Sets the current font sprite from a 
//sprite you have loaded in your code.
//Pass NULL to reset to default.
void SLK_draw_rgb_set_font_sprite(SLK_RGB_sprite *font)
{
   if(font==NULL)
   {
      text_sprite_rgb = text_sprite_rgb_default;
      return;
   }

   text_sprite_rgb = font;
}

//Clears the draw target to the color specified
//by SKL_draw_rgb_set_clear_color.
void SLK_draw_rgb_clear()
{
   for(int i = 0;i<target_rgb->width*target_rgb->height;i++)
      target_rgb->data[i] = target_rgb_clear;
}

//Draws a single pixel to the draw target.
void SLK_draw_rgb_color(int x, int y, SLK_Color color)
{
   if(color.a&&INBOUNDS(0,target_rgb->width,x)&&INBOUNDS(0,target_rgb->height,y))
      target_rgb->data[y*target_rgb->width+x] = color;
}

//Draws a string to the draw target.
//Color and scale must be specified.
void SLK_draw_rgb_string(int x, int y, int scale, const char *text, SLK_Color color)
{
   int x_dim = text_sprite_rgb->width/16;
   int y_dim = text_sprite_rgb->height/6;
   int sx = 0;
   int sy = 0;

   for(int i = 0;text[i];i++)
   {
      if(text[i]=='\n')
      {
         sx = 0;
         sy+=y_dim*scale;
         continue;
      }

      int ox = (text[i]-32)&15;
      int oy = (text[i]-32)/16;
      for(int y_ = 0;y_<y_dim;y_++)
      {
         for(int x_ = 0;x_<x_dim;x_++)
         {
            if(!text_sprite_rgb->data[(y_+oy*y_dim)*text_sprite_rgb->width+x_+ox*x_dim].a)
               continue;
            for(int m = 0;m<scale;m++)
               for(int o = 0;o<scale;o++)
                  SLK_draw_rgb_color(x+sx+(x_*scale)+o,y+sy+(y_*scale)+m,color);
         }
      }
      sx+=x_dim*scale;
   }
}

//Draws a string to the draw target.
//Draws the text directly from the font sprite instead of
//coloring visible pixels.
void SLK_draw_rgb_image_string(int x, int y, int scale, const char *text)
{
   int x_dim = text_sprite_rgb->width/16;
   int y_dim = text_sprite_rgb->height/6;
   int sx = 0;
   int sy = 0;

   for(int i = 0;text[i];i++)
   {
      if(text[i]=='\n')
      {
         sx = 0; 
         sy+=y_dim*scale;
         continue;
      }

      int ox = (text[i]-32)&15;
      int oy = (text[i]-32)/16;
      for(int y_ = 0;y_<y_dim;y_++)
      {
         for(int x_ = 0;x_<x_dim;x_++)
         {
            SLK_Color color = text_sprite_rgb->data[(y_+oy*y_dim)*text_sprite_rgb->width+x_+ox*x_dim];
            if(!color.a)
               continue;
            for(int m = 0;m<scale;m++)
               for(int o = 0;o<scale;o++)
                  SLK_draw_rgb_color(x+sx+(x_*scale)+o,y+sy+(y_*scale)+m,color);
         }
      }
      sx+=x_dim*scale;
   }
}

//Draws a sprite to the draw target.
//SoftLK does NOT do alpha blending in the software
//accelerated layers, but if you draw alpha,
//alpha blending will be applied to the layers below
//the current one.
void SLK_draw_rgb_sprite(const SLK_RGB_sprite *s, int x, int y)
{
   int draw_start_y = 0;
   int draw_start_x = 0;
   int draw_end_x = s->width;
   int draw_end_y = s->height;

   if(x<0)
      draw_start_x = -x;
   if(y<0)
      draw_start_y = -y;
   if(x+draw_end_x>target_rgb->width)
      draw_end_x = s->width+(target_rgb->width-x-draw_end_x);
   if(y+draw_end_y>target_rgb->height)
      draw_end_y = s->height+(target_rgb->height-y-draw_end_y);
    
   for(int y1 = draw_start_y;y1<draw_end_y;y1++)
   {
      for(int x1 = draw_start_x;x1<draw_end_x;x1++)
      {
         SLK_Color c = s->data[y1*s->width+x1];
         if(c.a)
            target_rgb->data[(y1+y)*target_rgb->width+x1+x] = c;
      }
   }
}

//Draws a specified part of a sprite, can be
//used for sprite sheets.
//There is no performance benefitt in doing
//so since rgb drawing is software accelerated 
//and the sprite data is located in RAM rather
//than in VRAM.
void SLK_draw_rgb_sprite_partial(const SLK_RGB_sprite *s, int x, int y, int ox, int oy, int width, int height)
{
   int draw_start_y = 0;
   int draw_start_x = 0;
   int draw_end_x = width;
   int draw_end_y = height;

   if(x<0)
      draw_start_x = -x;
   if(y<0)
      draw_start_y = -y;
   if(x+draw_end_x>target_rgb->width)
      draw_end_x = width+(target_rgb->width-x-draw_end_x);
   if(y+draw_end_y>target_rgb->height)
      draw_end_y = height+(target_rgb->height-y-draw_end_y);
    
   for(int y1 = draw_start_y;y1<draw_end_y;y1++)
   {
      for(int x1 = draw_start_x;x1<draw_end_x;x1++)
      {
         SLK_Color c = s->data[(y1+oy)*s->width+x1+ox];
         int index = (y1+y)*target_rgb->width+x1+x;
         if(c.a)
            target_rgb->data[index] = c;
      }
   }
}

//Draws a flipped sprite.
//Pass SLK_FLIP_VERTICAL for vertical flipping and
//SLK_FLIP_HORIZONTAL for horizontal flipping.
void SLK_draw_rgb_sprite_flip(const SLK_RGB_sprite *s, int x, int y, SLK_flip flip)
{
   int draw_start_y = 0;
   int draw_start_x = 0;
   int draw_end_x = s->width;
   int draw_end_y = s->height;

   if(x<0)
      draw_start_x = -x;
   if(y<0)
      draw_start_y = -y;
   if(x+draw_end_x>target_rgb->width)
      draw_end_x = s->width+(target_rgb->width-x-draw_end_x);
   if(y+draw_end_y>target_rgb->height)
      draw_end_y = s->height+(target_rgb->height-y-draw_end_y);

   switch(flip)
   {
   case SLK_FLIP_NONE:
      for(int y1 = draw_start_y;y1<draw_end_y;y1++)
      {
         for(int x1 = draw_start_x;x1<draw_end_x;x1++)
         {
            SLK_Color c = s->data[y1*s->width+x1];
            if(c.a)
               target_rgb->data[(y1+y)*target_rgb->width+x1+x] = c;
         }
      }
      break;
   case SLK_FLIP_VERTICAL:
      for(int y1 = draw_start_y;y1<draw_end_y;y1++)
      {
         for(int x1 = draw_start_x;x1<draw_end_x;x1++)
         {
            SLK_Color c = s->data[(s->height-y1-1)*s->width+x1];
            if(c.a)
               target_rgb->data[(y1+y)*target_rgb->width+x1+x] = c;
         }
      }
      break;
   case SLK_FLIP_HORIZONTAL:
      for(int y1 = draw_start_y;y1<draw_end_y;y1++)
      {
         for(int x1 = draw_start_x;x1<draw_end_x;x1++)
         {
            SLK_Color c = s->data[y1*s->width+(s->width-x1-1)];
            if(c.a)
               target_rgb->data[(y1+y)*target_rgb->width+x1+x] = c;
         }
      }
      break;
   case SLK_FLIP_VERTHOR:
      for(int y1 = draw_start_y;y1<draw_end_y;y1++)
      {
         for(int x1 = draw_start_x;x1<draw_end_x;x1++)
         {
            SLK_Color c = s->data[(s->height-y1-1)*s->width+(s->width-x1-1)];
            if(c.a)
               target_rgb->data[(y1+y)*target_rgb->width+x1+x] = c;
         }
      }
      break;
   }
}

//Draws a line between to points
//using the Bresenham line drawing algorythm.
void SLK_draw_rgb_line(int x0, int y0, int x1, int y1, SLK_Color color)
{
   if(x0>x1||y0>y1)
   {
      SWAP(x0,x1);
      SWAP(y0,y1);
   }
   int dx = x1-x0;
   int ix = (dx>0)-(dx<0);
   dx = abs(dx)<<1;
   int dy = y1-y0;
   int iy = (dy>0)-(dy<0);
   dy = abs(dy)<<1;
   SLK_draw_rgb_color(x0,y0,color);

   if(dx>=dy)
   {
      int error = dy-(dx>>1);
      while(x0!=x1)
      {
         if(error>0||(!error&&ix>0))
         {
            error-=dx;
            y0+=iy;
         }

         error+=dy;
         x0+=ix;

         SLK_draw_rgb_color(x0,y0,color);
      }
   }
   else
   {
      int error = dx-(dy>>1);

      while(y0!=y1)
      {
         if(error>0||(!error&&iy>0))
         {
            error-=dy;
            x0+=ix;
         }

         error+=dx;
         y0+=iy;

         SLK_draw_rgb_color(x0,y0,color);
      }
   }
}

//Draws a line between two points up two but not including the second point
//with fixed x coordinates.
void SLK_draw_rgb_vertical_line(int x, int y0, int y1, SLK_Color color)
{
   if(x<0||x>=target_rgb->width||y0>=target_rgb->height||y1<0)
      return;
   if(y0<0)
      y0 = 0;
   if(y1>target_rgb->height)
      y1 = target_rgb->height;

   for(int y = y0;y<y1;y++)
      target_rgb->data[y*target_rgb->width+x] = color;
}

//Draws a line between two points up two but not including the second point
//with fixed y coordinates.
void SLK_draw_rgb_horizontal_line(int x0, int x1, int y, SLK_Color color)
{
   if(y<0||y>=target_rgb->height||x0>=target_rgb->width||x1<0)
      return;
   if(x0<0)
      x0 = 0;
   if(x1>target_rgb->width)
      x1 = target_rgb->width;

   for(int x = x0;x<x1;x++)
      target_rgb->data[y*target_rgb->width+x] = color;
}

//Draws the outline of a colored rectangle.
void SLK_draw_rgb_rectangle(int x, int y, int width, int height, SLK_Color color)
{
   SLK_draw_rgb_horizontal_line(x,x+width,y,color);
   SLK_draw_rgb_horizontal_line(x,x+width,y+height-1,color);
   SLK_draw_rgb_vertical_line(x,y,y+height,color);
   SLK_draw_rgb_vertical_line(x+width-1,y,y+height-1,color);
}

//Draws a solid colored rectangle.
void SLK_draw_rgb_fill_rectangle(int x, int y, int width, int height, SLK_Color color)
{
   int draw_start_y = 0;
   int draw_start_x = 0;
   int draw_end_x = width;
   int draw_end_y = height;

   if(x<0)
      draw_start_x = -x;
   if(y<0)
      draw_start_y = -y;
   if(x+draw_end_x>target_rgb->width)
      draw_end_x = width+(target_rgb->width-x-draw_end_x);
   if(y+draw_end_y>target_rgb->height)
      draw_end_y = height+(target_rgb->height-y-draw_end_y);
    
   for(int y1 = draw_start_y;y1<draw_end_y;y1++)
      for(int x1 = draw_start_x;x1<draw_end_x;x1++)
         target_rgb->data[(y1+y)*target_rgb->width+x1+x] = color;
}

//Draws the outline of a colored circle.
void SLK_draw_rgb_circle(int x, int y, int radius, SLK_Color color)
{
   int x_ = 0;
   int y_ = radius;
   int d = 1-radius;

   SLK_draw_rgb_color(x,y+radius,color);
   SLK_draw_rgb_color(x,y-radius,color);
   SLK_draw_rgb_color(x+radius,y,color);
   SLK_draw_rgb_color(x-radius,y,color);

   while(x_<y_)
   {
      if(d<0)
      {
         d = d+2*x_+3;
         x_+=1;
      }
      else
      {
         d = d+2*(x_-y_)+5;
         x_+=1;
         y_-=1;
      }

      SLK_draw_rgb_color(x+x_,y+y_,color);
      SLK_draw_rgb_color(x+x_,y-y_,color);
      SLK_draw_rgb_color(x-x_,y+y_,color);
      SLK_draw_rgb_color(x-x_,y-y_,color);

      SLK_draw_rgb_color(x+y_,y+x_,color);
      SLK_draw_rgb_color(x+y_,y-x_,color);
      SLK_draw_rgb_color(x-y_,y+x_,color);
      SLK_draw_rgb_color(x-y_,y-x_,color);
   }
}

//Draws a solid colored circle.
void SLK_draw_rgb_fill_circle(int x, int y, int radius, SLK_Color color)
{
   int x_ = 0;
   int y_ = radius;
   int d = 1-radius;

   SLK_draw_rgb_horizontal_line(x-radius,x+radius,y,color);

   while(x_<y_)
   {
      if(d<0)
      {
         d = d+2*x_+3;
         x_+=1;
      }
      else
      {
         d = d+2*(x_-y_)+5;
         x_+=1;
         y_-=1;
      }

      SLK_draw_rgb_horizontal_line(x-x_,x+x_,y+y_,color);
      SLK_draw_rgb_horizontal_line(x-x_,x+x_,y-y_,color);
      SLK_draw_rgb_horizontal_line(x-y_,x+y_,y+x_,color);
      SLK_draw_rgb_horizontal_line(x-y_,x+y_,y-x_,color);
   }
}

#endif
//-------------------------------------
