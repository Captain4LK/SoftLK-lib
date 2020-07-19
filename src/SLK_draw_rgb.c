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
#include "SLK_draw_rgb_i.h"
//-------------------------------------

//#defines
#define INBOUNDS(LOWER,UPPER,NUMBER) \
            ((unsigned)(NUMBER-LOWER)<(UPPER-LOWER))
#define SIGNUM(NUM) \
   NUM==0?0:(NUM<0?-1:1)
#define SWAP(x,y) \
            { (x)=(x)^(y); (y)=(x)^(y); (x)=(x)^(y); }
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
//-------------------------------------

//Function prototypes
//-------------------------------------

//Function implementations

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
   int sx = 0;
   int sy = 0;

	for (int i = 0;text[i];i++)
	{
		if (text[i]=='\n')
		{
			sx = 0; 
         sy+=8*scale;
		}
		else
		{
			int ox = (text[i]-32) % 16;
			int oy = (text[i]-32) / 16;

			if(scale>1)
			{
				for(int x_ = 0;x_<8;x_++)
					for(int y_ = 0;y_<8;y_++)
						if(text_sprite_rgb->data[(y_+oy*8)*128+x_+ox*8].a)
							for(int is = 0;is<scale;is++)
								for(int js = 0;js<scale;js++)
									SLK_draw_rgb_color(x+sx+(x_*scale)+is,y+sy+(y_*scale)+js,color);
			}
			else
			{
				for(int x_ = 0; x_ < 8; x_++)
					for(int y_ = 0; y_ < 8; y_++)
						if(text_sprite_rgb->data[(y_+oy*8)*128+x_+ox*8].a)
						 	SLK_draw_rgb_color(x+sx+x_,y+sy+y_,color);
			}
			sx += 8*scale;
		}
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
    
   for(int x1 = draw_start_x;x1<draw_end_x;x1++)
   {
      for(int y1 = draw_start_y;y1<draw_end_y;y1++)
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
   for(int tx = 0; tx < width; tx++)
   {
      for(int ty = 0; ty < height; ty++)
      {
         SLK_Color c = SLK_rgb_sprite_get_pixel(s, tx + ox, ty + oy);
         SLK_draw_rgb_color(x + tx, y +ty, c);
      }
   }
}

//Draws a flipped sprite.
//Pass SLK_FLIP_VERTICAL for vertical flipping and
//SLK_FLIP_HORIZONTAL for horizontal flipping.
void SLK_draw_rgb_sprite_flip(const SLK_RGB_sprite *s, int x, int y, int flip)
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

   if(flip==SLK_FLIP_NONE)
   {
      for(int x1 = draw_start_x;x1<draw_end_x;x1++)
      {
         for(int y1 = draw_start_y;y1<draw_end_y;y1++)
         {
            SLK_Color c = s->data[y1*s->width+x1];
            if(c.a)
               target_rgb->data[(y1+y)*target_rgb->width+x1+x] = c;
         }
      }
   }
   else if(flip==SLK_FLIP_VERTICAL)
   {
      for(int x1 = draw_start_x;x1<draw_end_x;x1++)
      {
         for(int y1 = draw_start_y;y1<draw_end_y;y1++)
         {
            SLK_Color c = s->data[(s->height-y1-1)*s->width+x1];
            if(c.a)
               target_rgb->data[(y1+y)*target_rgb->width+x1+x] = c;
         }
      }
   }
   else if(flip==SLK_FLIP_HORIZONTAL)
   {
      for(int x1 = draw_start_x;x1<draw_end_x;x1++)
      {
         for(int y1 = draw_start_y;y1<draw_end_y;y1++)
         {
            SLK_Color c = s->data[y1*s->width+(s->width-x1-1)];
            if(c.a)
               target_rgb->data[(y1+y)*target_rgb->width+x1+x] = c;
         }
      }
   }
   else if(flip==(SLK_FLIP_VERTICAL|SLK_FLIP_HORIZONTAL))
   {
      for(int x1 = draw_start_x;x1<draw_end_x;x1++)
      {
         for(int y1 = draw_start_y;y1<draw_end_y;y1++)
         {
            SLK_Color c = s->data[(s->height-y1-1)*s->width+(s->width-x1)];
            if(c.a)
               target_rgb->data[(y1+y)*target_rgb->width+x1+x] = c;
         }
      }
   }
}

//Draws a line between to points
//using the Bresenham line drawing algorythm.
void SLK_draw_rgb_line(int x0, int y0, int x1, int y1, SLK_Color color)
{
   int changed = 0;
   int x = x0;
   int y = y0;

   int dx = abs(x1-x0);
   int dy = abs(y1-y0);

   
   int sign_x = SIGNUM(x1-x0);
   int sign_y = SIGNUM(y1-y0);

   if(dy>dx)
   {
      SWAP(dx,dy);
      changed = 1;
   }

    int error = 2*dy-dx;

    for(int i = 1;i<=dx;i++)
    {
        SLK_draw_rgb_color(x,y,color);

        while(error>=0)
        {
            if(changed)
                x+=1;
            else
                y+=1;
            error-=2*dx;
        }

        if(changed)
            y+=sign_y;
        else
            x+=sign_x;
        error+=2*dy;
    }

}

//Draws a line between two points
//with fixed x coordinates.
void SLK_draw_rgb_vertical_line(int x, int y0, int y1, SLK_Color color)
{
   for(int y = y0;y<y1;y++)
      SLK_draw_rgb_color(x,y,color);
}

//Draws a line between two points
//with fixed y coordinates
void SLK_draw_rgb_horizontal_line(int x0, int x1, int y, SLK_Color color)
{
   for(int x = x0;x<x1;x++)
      SLK_draw_rgb_color(x,y,color);
}

//Draws the outline of a colored
//rectangle.
void SLK_draw_rgb_rectangle(int x, int y, int width, int height, SLK_Color color)
{
   for(int i = x;i<x+width;i++)
   {
      SLK_draw_rgb_color(i,y,color);
      SLK_draw_rgb_color(i,y+height-1,color);
   }

   for(int i = y;i<y+height;i++)
   {
      SLK_draw_rgb_color(x,i,color);
      SLK_draw_rgb_color(x+width-1,i,color);
   }
}

//Draws a solid colored rectangle.
void SLK_draw_rgb_fill_rectangle(int x, int y, int width, int height, SLK_Color color)
{
   for(int x_ = x;x_<x+width;x_++)
      for(int y_ = y;y_<y+height;y_++)
         SLK_draw_rgb_color(x_,y_,color);
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
//-------------------------------------
