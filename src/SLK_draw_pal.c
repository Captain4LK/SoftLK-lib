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
#include <stdlib.h>
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

#define SIGNUM(NUM) \
   NUM==0?0:(NUM<0?-1:1)
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
static SLK_Paxel target_pal_clear;
static SLK_Pal_sprite *text_sprite_pal;
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
void SLK_draw_pal_set_clear_paxel(SLK_Paxel paxel)
{
   target_pal_clear = paxel;
}

//Clears the target to the color set by 
//SLK_draw_pal_set_clear_paxel.
void SLK_draw_pal_clear()
{
   for(int i = 0;i<target_pal->width*target_pal->height;i++)
      target_pal->data[i] = target_pal_clear;
}

//Draws a single paxel to the draw target.
void SLK_draw_pal_paxel(int x, int y, SLK_Paxel paxel)
{
   if(INBOUNDS(0,target_pal->width,x)&&INBOUNDS(0,target_pal->height,y))
   {
      int index = y*target_pal->width+x;
      target_pal->data[index].index = (target_pal->data[index].index&paxel.mask)|paxel.index;
   }
}

//Draws a string to the draw target.
//Color and scale must be specified.
void SLK_draw_pal_string(int x, int y, int scale, const char *text, SLK_Paxel paxel)
{
   int x_dim = text_sprite_pal->width/16;
   int y_dim = text_sprite_pal->height/6;
   int x_len = x_dim*16;
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

      for(int x_ = 0;x_<x_dim;x_++)
      {
         for(int y_ = 0;y_<y_dim;y_++)
         {
            if(!text_sprite_pal->data[(y_+oy*y_dim)*x_len+x_+ox*x_dim].mask)
            {
               for(int o = 0;o<scale;o++)
               {
                  for(int m = 0;m<scale;m++)
                  {
                     SLK_draw_pal_paxel(x+sx+(x_*scale)+o,y+sy+(y_*scale)+m,paxel);
                  }
               }
            }
         }
      }
      sx += x_dim*scale;
	}
}

//Draws a sprite to the draw target.
//Uses bit blitting for faster drawing.
void SLK_draw_pal_sprite(const SLK_Pal_sprite *s, int x, int y)
{
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
    
   for(int y1 = draw_start_y;y1<draw_end_y;y1++)
   {
      for(int x1 = draw_start_x;x1<draw_end_x;x1++)
      {
         SLK_Paxel p = s->data[y1*s->width+x1];
         int index = (y1+y)*target_pal->width+x1+x;
         target_pal->data[index].index = (target_pal->data[index].index&p.mask)|p.index;
      }
   }
}

//Draws a specified part of a sprite to the draw target.
//This could be used for spritesheet drawing, but since the pal 
//drawing system is software accelerated, there is no speed penality
//when splitting the spritesheet into individual sprites.
void SLK_draw_pal_sprite_partial(const SLK_Pal_sprite *s, int x, int y, int ox, int oy, int width, int height)
{

   int draw_start_y = 0;
   int draw_start_x = 0;
   int draw_end_x = width;
   int draw_end_y = height;

   if(x<0)
      draw_start_x = -x;
   if(y<0)
      draw_start_y = -y;
   if(x+draw_end_x>target_pal->width)
      draw_end_x = s->width+(target_pal->width-x-draw_end_x);
   if(y+draw_end_y>target_pal->height)
      draw_end_y = s->height+(target_pal->height-y-draw_end_y);
    
   for(int y1 = draw_start_y;y1<draw_end_y;y1++)
   {
      for(int x1 = draw_start_x;x1<draw_end_x;x1++)
      {
         SLK_Paxel p = s->data[(y1+oy)*s->width+x1+ox];
         int index = (y1+y)*target_pal->width+x1+x;
         target_pal->data[index].index = (target_pal->data[index].index&p.mask)|p.index;
      }
   }
}

//Draws a flipped sprite to the draw target.
//For vertical flipping pass SLK_FLIP_VERTICAL,
//for horizontal flipping pass SLK_FLIP_HORIZONTAL
//(Note: The values can be or'd together).
void SLK_draw_pal_sprite_flip(const SLK_Pal_sprite *s, int x, int y, int flip)
{
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

   switch(flip)
   {
   case SLK_FLIP_NONE:
      for(int y1 = draw_start_y;y1<draw_end_y;y1++)
      {
         for(int x1 = draw_start_x;x1<draw_end_x;x1++)
         {
            SLK_Paxel p = s->data[y1*s->width+x1];
            int index = (y1+y)*target_pal->width+x1+x;
            target_pal->data[index].index = (target_pal->data[index].index&p.mask)|p.index;
         }
      }
      break;
   case SLK_FLIP_VERTICAL:
      for(int y1 = draw_start_y;y1<draw_end_y;y1++)
      {
         for(int x1 = draw_start_x;x1<draw_end_x;x1++)
         {
            SLK_Paxel p = s->data[(s->height-y1-1)*s->width+x1];
            int index = (y1+y)*target_pal->width+x1+x;
            target_pal->data[index].index = (target_pal->data[index].index&p.mask)|p.index;
         }
      }
      break;
   case SLK_FLIP_HORIZONTAL:
      for(int y1 = draw_start_y;y1<draw_end_y;y1++)
      {
         for(int x1 = draw_start_x;x1<draw_end_x;x1++)
         {
            SLK_Paxel p = s->data[y1*s->width+(s->width-x1-1)];
            int index = (y1+y)*target_pal->width+x1+x;
            target_pal->data[index].index = (target_pal->data[index].index&p.mask)|p.index;
         }
      }
      break;
   case (SLK_FLIP_VERTICAL|SLK_FLIP_HORIZONTAL):
      for(int y1 = draw_start_y;y1<draw_end_y;y1++)
      {
         for(int x1 = draw_start_x;x1<draw_end_x;x1++)
         {
            SLK_Paxel p = s->data[(s->height-y1-1)*s->width+(s->width-x1)];
            int index = (y1+y)*target_pal->width+x1+x;
            target_pal->data[index].index = (target_pal->data[index].index&p.mask)|p.index;
         }
      }
      break;
   }
}

//Draws a colored line between 2 points using
//the Bresenham line drawing algorythm.
void SLK_draw_pal_line(int x0, int y0, int x1, int y1, SLK_Paxel paxel)
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
   SLK_draw_pal_paxel(x0,y0,paxel);

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

         SLK_draw_pal_paxel(x0,y0,paxel);
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

         SLK_draw_pal_paxel(x0,y0,paxel);
      }
   }
}

//Draws a line between to points up to but not including the second point,
//with a fixed x value.
void SLK_draw_pal_vertical_line(int x, int y0, int y1, SLK_Paxel paxel)
{
   if(x<0||x>=target_pal->width||y0>=target_pal->height||y1<0)
      return;
   if(y0<0)
      y0 = 0;
   if(y1>target_pal->height)
      y1 = target_pal->height;

   for(int y = y0;y<y1;y++)
      target_pal->data[y*target_pal->width+x] = paxel;
}

//Draws a line between to points up to but not including the second point,
//with a fixed y value.
void SLK_draw_pal_horizontal_line(int x0, int x1, int y, SLK_Paxel paxel)
{
   if(y<0||y>=target_pal->height||x0>=target_pal->width||x1<0)
      return;
   if(x0<0)
      x0 = 0;
   if(x1>target_pal->width)
      x1 = target_pal->width;

   for(int x = x0;x<x1;x++)
      target_pal->data[y*target_pal->width+x] = paxel;
}

//Draws the outline of a colored rectangle.
void SLK_draw_pal_rectangle(int x, int y, int width, int height, SLK_Paxel paxel)
{
   SLK_draw_pal_horizontal_line(x,x+width,y,paxel);
   SLK_draw_pal_horizontal_line(x,x+width,y+height-1,paxel);
   SLK_draw_pal_vertical_line(x,y,y+height,paxel);
   SLK_draw_pal_vertical_line(x+width-1,y,y+height-1,paxel);
}

//Draws a colored filled rectangle.
void SLK_draw_pal_fill_rectangle(int x, int y, int width, int height, SLK_Paxel paxel)
{
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
    
   for(int x1 = draw_start_x;x1<draw_end_x;x1++)
   {
      for(int y1 = draw_start_y;y1<draw_end_y;y1++)
      {
         int index = (y1+y)*target_pal->width+x1+x;
         target_pal->data[index].index = (target_pal->data[index].index&paxel.mask)|paxel.index;
      }
   }
}

//Draws the outline of a colored circle.
void SLK_draw_pal_circle(int x, int y, int radius, SLK_Paxel paxel)
{
   int x_ = 0;
   int y_ = radius;
   int d = 1-radius;

   SLK_draw_pal_paxel(x,y+radius,paxel);
   SLK_draw_pal_paxel(x,y-radius,paxel);
   SLK_draw_pal_paxel(x+radius,y,paxel);
   SLK_draw_pal_paxel(x-radius,y,paxel);

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

      SLK_draw_pal_paxel(x+x_,y+y_,paxel);
      SLK_draw_pal_paxel(x+x_,y-y_,paxel);
      SLK_draw_pal_paxel(x-x_,y+y_,paxel);
      SLK_draw_pal_paxel(x-x_,y-y_,paxel);

      SLK_draw_pal_paxel(x+y_,y+x_,paxel);
      SLK_draw_pal_paxel(x+y_,y-x_,paxel);
      SLK_draw_pal_paxel(x-y_,y+x_,paxel);
      SLK_draw_pal_paxel(x-y_,y-x_,paxel);
   }
}

//Draws a colored filled circle.
void SLK_draw_pal_fill_circle(int x, int y, int radius, SLK_Paxel paxel)
{
   int x_ = 0;
   int y_ = radius;
   int d = 1-radius;

   SLK_draw_pal_horizontal_line(x-radius,x+radius,y,paxel);

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

      SLK_draw_pal_horizontal_line(x-x_,x+x_,y+y_,paxel);
      SLK_draw_pal_horizontal_line(x-x_,x+x_,y-y_,paxel);
      SLK_draw_pal_horizontal_line(x-y_,x+y_,y+x_,paxel);
      SLK_draw_pal_horizontal_line(x-y_,x+y_,y-x_,paxel);
   }
}
//-------------------------------------

#undef SWAP
#undef SIGNUM
#undef INBOUNDS
