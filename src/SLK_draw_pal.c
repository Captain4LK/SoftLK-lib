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
#include <string.h>
//-------------------------------------

//Internal includes
#include "../include/SLK/SLK_types.h"
#include "../include/SLK/SLK_functions.h"
#include "SLK_draw_pal_i.h"
//-------------------------------------

//#defines
#define SWAP(x,y) \
            { (x)=(x)^(y); (y)=(x)^(y); (x)=(x)^(y); }

#define INBOUNDS(LOWER,UPPER,NUMBER) \
            ((unsigned)(NUMBER-LOWER)<(UPPER-LOWER))
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
static uint8_t target_pal_clear;
static const SLK_Pal_sprite *text_sprite_pal;
static SLK_Pal_sprite *text_sprite_pal_default;

SLK_Pal_sprite *target_pal;
SLK_Pal_sprite *target_pal_default;
//-------------------------------------

//Function prototypes
//-------------------------------------

//Function implementations

//Returns the target for drawing operations.
//Usefull for getting the default draw target.
SLK_Pal_sprite *SLK_draw_pal_get_target()
{
   return target_pal;
}

//Sets the target for drawing operations.
//Used for drawing to sprites, for creating spritesheets
//SLK_pal_sprite_copy(_partial) is advised.
void SLK_draw_pal_set_target(SLK_Pal_sprite *s)
{
   if(s==NULL)
   {
      target_pal = target_pal_default;
      return;
   }

   target_pal = s;
}

//Loads a new font sprite.
//Replaces the default font sprite.
void SLK_draw_pal_load_font(const char *path)
{
   SLK_pal_sprite_destroy(text_sprite_pal_default);
   text_sprite_pal_default = SLK_pal_sprite_load(path);
   text_sprite_pal = text_sprite_pal_default;
}

//Sets the current font sprite from a 
//sprite you have loaded in your code.
//Pass NULL to reset to default.
void SLK_draw_pal_set_font_sprite(SLK_Pal_sprite *font)
{
   if(font==NULL)
   {
      text_sprite_pal = text_sprite_pal_default;
      return;
   }

   text_sprite_pal = font;
}

//Sets the color wich the target is to be cleared to
//when calling SLK_draw_pal_clear.
void SLK_draw_pal_set_clear_index(uint8_t index)
{
   target_pal_clear = index;
}

//Clears the target to the color set by 
//SLK_draw_pal_set_clear_index.
void SLK_draw_pal_clear()
{
   memset(target_pal->data,target_pal_clear,sizeof(*target_pal->data)*target_pal->width*target_pal->height);
}

//Draws a single paxel to the draw target.
void SLK_draw_pal_index(int x, int y, uint8_t index)
{
   if(!index)
      return;

   if(INBOUNDS(0,target_pal->width,x)&&INBOUNDS(0,target_pal->height,y))
   {
      int ind = y*target_pal->width+x;
      target_pal->data[ind]= index;
   }
}

//Draws a string to the draw target.
//Color and scale must be specified.
void SLK_draw_pal_string(int x, int y, int scale, const char *text, uint8_t index)
{
   if(!index)
      return;

   int x_dim = text_sprite_pal->width/16;
   int y_dim = text_sprite_pal->height/6;
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
            if(text_sprite_pal->data[(y_+oy*y_dim)*text_sprite_pal->width+x_+ox*x_dim])
               continue;
            for(int m = 0;m<scale;m++)
               for(int o = 0;o<scale;o++)
                  SLK_draw_pal_index(x+sx+(x_*scale)+o,y+sy+(y_*scale)+m,index);
         }
      }
      sx+=x_dim*scale;
   }
}

//Draws a sprite to the draw target.
void SLK_draw_pal_sprite(const SLK_Pal_sprite *s, int x, int y)
{
   //Clip source sprite
   int draw_start_y = 0;
   int draw_start_x = 0;
   int draw_end_x = s->width;
   int draw_end_y = s->height;
   if(x<0)
      draw_start_x = -x;
   if(y<0)
      draw_start_y = -y;
   if(x+draw_end_x>target_pal->width)
      draw_end_x = s->width+(target_pal->width-x-draw_end_x);
   if(y+draw_end_y>target_pal->height)
      draw_end_y = s->height+(target_pal->height-y-draw_end_y);

   //Clip dst sprite
   x = x<0?0:x;
   y = y<0?0:y;

   const uint8_t *src = &s->data[draw_start_x+draw_start_y*s->width];
   uint8_t *dst = &target_pal->data[x+y*target_pal->width];
   int src_step = -(draw_end_x-draw_start_x)+s->width;
   int dst_step = target_pal->width-(draw_end_x-draw_start_x);
    
   for(int y1 = draw_start_y;y1<draw_end_y;y1++,dst+=dst_step,src+=src_step)
      for(int x1 = draw_start_x;x1<draw_end_x;x1++,src++,dst++)
         *dst = *src?*src:*dst;
}

//Draws a specified part of a sprite to the draw target.
//This could be used for spritesheet drawing, but since the pal 
//drawing system is software accelerated, there is no speed penality
//when splitting the spritesheet into individual sprites.
void SLK_draw_pal_sprite_partial(const SLK_Pal_sprite *s, int x, int y, int ox, int oy, int width, int height)
{
   //Clip source sprite
   int draw_start_y = 0;
   int draw_start_x = 0;
   int draw_end_x = width;
   int draw_end_y = height;
   if(x<0)
      draw_start_x = -x;
   if(y<0)
      draw_start_y = -y;
   if(x+draw_end_x>target_pal->width)
      draw_end_x = width+(target_pal->width-x-draw_end_x);
   if(y+draw_end_y>target_pal->height)
      draw_end_y = height+(target_pal->height-y-draw_end_y);

   //Clip dst sprite
   x = x<0?0:x;
   y = y<0?0:y;

   const uint8_t *src = &s->data[draw_start_x+ox+(draw_start_y+oy)*s->width];
   uint8_t *dst = &target_pal->data[x+y*target_pal->width];
   int src_step = -(draw_end_x-draw_start_x)+s->width;
   int dst_step = target_pal->width-(draw_end_x-draw_start_x);
    
   for(int y1 = draw_start_y;y1<draw_end_y;y1++,dst+=dst_step,src+=src_step)
      for(int x1 = draw_start_x;x1<draw_end_x;x1++,src++,dst++)
         *dst = *src?*src:*dst;
}

//Draws a flipped sprite to the draw target.
//For vertical flipping pass SLK_FLIP_VERTICAL,
//for horizontal flipping pass SLK_FLIP_HORIZONTAL
//(Note: The values can be or'd together).
void SLK_draw_pal_sprite_flip(const SLK_Pal_sprite *s, int x, int y, SLK_flip flip)
{
   //Clip source sprite
   int draw_start_y = 0;
   int draw_start_x = 0;
   int draw_end_x = s->width;
   int draw_end_y = s->height;
   if(x<0)
      draw_start_x = -x;
   if(y<0)
      draw_start_y = -y;
   if(x+draw_end_x>target_pal->width)
      draw_end_x = s->width+(target_pal->width-x-draw_end_x);
   if(y+draw_end_y>target_pal->height)
      draw_end_y = s->height+(target_pal->height-y-draw_end_y);

   //Clip dst sprite
   x = x<0?0:x;
   y = y<0?0:y;

   const uint8_t *src = &s->data[0];
   uint8_t *dst = &target_pal->data[x+y*target_pal->width];
   int src_step = -(draw_end_x-draw_start_x)+s->width;
   int dst_step = target_pal->width-(draw_end_x-draw_start_x);

   switch(flip)
   {
   case SLK_FLIP_NONE:
      src+=draw_start_x+draw_start_y*s->width;
      src_step = -(draw_end_x-draw_start_x)+s->width;
      dst_step = target_pal->width-(draw_end_x-draw_start_x);
      for(int y1 = draw_start_y;y1<draw_end_y;y1++,dst+=dst_step,src+=src_step)
         for(int x1 = draw_start_x;x1<draw_end_x;x1++,src++,dst++)
            *dst = *src?*src:*dst;
      break;
   case SLK_FLIP_VERTICAL:
      src+=draw_start_x+s->width*(s->height-draw_start_y-1);
      src_step = -(draw_end_x-draw_start_x)-s->width;
      dst_step = target_pal->width-(draw_end_x-draw_start_x);
      for(int y1 = draw_start_y;y1<draw_end_y;y1++,dst+=dst_step,src+=src_step)
         for(int x1 = draw_start_x;x1<draw_end_x;x1++,src++,dst++)
            *dst = *src?*src:*dst;
      break;
   case SLK_FLIP_HORIZONTAL:
      src+=draw_start_y*s->width-draw_start_x+s->width-1;
      src_step = (draw_end_x-draw_start_x)+s->width;
      dst_step = target_pal->width-(draw_end_x-draw_start_x);
      for(int y1 = draw_start_y;y1<draw_end_y;y1++,dst+=dst_step,src+=src_step)
         for(int x1 = draw_start_x;x1<draw_end_x;x1++,src--,dst++)
            *dst = *src?*src:*dst;
      break;
   case SLK_FLIP_VERTHOR:
      src+=s->width*(s->height-draw_start_y-1);
      src+=-draw_start_x+s->width-1;
      src_step = (draw_end_x-draw_start_x)-s->width;
      dst_step = target_pal->width-(draw_end_x-draw_start_x);
      for(int y1 = draw_start_y;y1<draw_end_y;y1++,dst+=dst_step,src+=src_step)
         for(int x1 = draw_start_x;x1<draw_end_x;x1++,src--,dst++)
            *dst = *src?*src:*dst;
      break;
   }
}

//Draws a colored line between 2 points using
//the Bresenham line drawing algorythm.
void SLK_draw_pal_line(int x0, int y0, int x1, int y1, uint8_t index)
{
   if(!index)
      return;

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
   SLK_draw_pal_index(x0,y0,index);

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

         SLK_draw_pal_index(x0,y0,index);
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

         SLK_draw_pal_index(x0,y0,index);
      }
   }
}

//Draws a line between to points up to but not including the second point,
//with a fixed x value.
void SLK_draw_pal_vertical_line(int x, int y0, int y1, uint8_t index)
{
   if(!index)
      return;

   if(x<0||x>=target_pal->width||y0>=target_pal->height||y1<0)
      return;
   if(y0<0)
      y0 = 0;
   if(y1>target_pal->height)
      y1 = target_pal->height;

   for(int y = y0;y<y1;y++)
      target_pal->data[y*target_pal->width+x] = index;
}

//Draws a line between to points up to but not including the second point,
//with a fixed y value.
void SLK_draw_pal_horizontal_line(int x0, int x1, int y, uint8_t index)
{
   if(!index)
      return;
   
   if(y<0||y>=target_pal->height||x0>=target_pal->width||x1<0)
      return;
   if(x0<0)
      x0 = 0;
   if(x1>target_pal->width)
      x1 = target_pal->width;

   uint8_t *dst = &target_pal->data[y*target_pal->width+x0];
   for(int x = x0;x<x1;x++,dst++)
      *dst = index;
}

//Draws the outline of a colored rectangle.
void SLK_draw_pal_rectangle(int x, int y, int width, int height, uint8_t index)
{
   if(!index)
      return;

   SLK_draw_pal_horizontal_line(x,x+width,y,index);
   SLK_draw_pal_horizontal_line(x,x+width,y+height-1,index);
   SLK_draw_pal_vertical_line(x,y,y+height,index);
   SLK_draw_pal_vertical_line(x+width-1,y,y+height-1,index);
}

//Draws a colored filled rectangle.
//The only function to not return when drawing with an
//index of 0, can thus be used for clearing parts of
//the target.
void SLK_draw_pal_fill_rectangle(int x, int y, int width, int height, uint8_t index)
{
   //Clip src rect
   int draw_start_y = 0;
   int draw_start_x = 0;
   int draw_end_x = width;
   int draw_end_y = height;

   if(x<0)
      draw_start_x = -x;
   if(y<0)
      draw_start_y = -y;
   if(x+draw_end_x>target_pal->width)
      draw_end_x = width+(target_pal->width-x-draw_end_x);
   if(y+draw_end_y>target_pal->height)
      draw_end_y = height+(target_pal->height-y-draw_end_y);
    
   //Clip dst rect
   x = x<0?0:x;
   y = y<0?0:y;

   uint8_t *dst = &target_pal->data[x+y*target_pal->width];
   int dst_step = target_pal->width-(draw_end_x-draw_start_x);
    
   for(int y1 = draw_start_y;y1<draw_end_y;y1++,dst+=dst_step)
      for(int x1 = draw_start_x;x1<draw_end_x;x1++,dst++)
         *dst = index;
}

//Draws the outline of a colored circle.
void SLK_draw_pal_circle(int x, int y, int radius, uint8_t index)
{
   if(!index)
      return;

   int x_ = 0;
   int y_ = radius;
   int d = 1-radius;

   SLK_draw_pal_index(x,y+radius,index);
   SLK_draw_pal_index(x,y-radius,index);
   SLK_draw_pal_index(x+radius,y,index);
   SLK_draw_pal_index(x-radius,y,index);

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

      SLK_draw_pal_index(x+x_,y+y_,index);
      SLK_draw_pal_index(x+x_,y-y_,index);
      SLK_draw_pal_index(x-x_,y+y_,index);
      SLK_draw_pal_index(x-x_,y-y_,index);

      SLK_draw_pal_index(x+y_,y+x_,index);
      SLK_draw_pal_index(x+y_,y-x_,index);
      SLK_draw_pal_index(x-y_,y+x_,index);
      SLK_draw_pal_index(x-y_,y-x_,index);
   }
}

//Draws a colored filled circle.
void SLK_draw_pal_fill_circle(int x, int y, int radius, uint8_t index)
{
   if(!index)
      return;

   int x_ = 0;
   int y_ = radius;
   int d = 1-radius;

   SLK_draw_pal_horizontal_line(x-radius,x+radius,y,index);

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

      SLK_draw_pal_horizontal_line(x-x_,x+x_,y+y_,index);
      SLK_draw_pal_horizontal_line(x-x_,x+x_,y-y_,index);
      SLK_draw_pal_horizontal_line(x-y_,x+y_,y+x_,index);
      SLK_draw_pal_horizontal_line(x-y_,x+y_,y-x_,index);
   }
}
//-------------------------------------
