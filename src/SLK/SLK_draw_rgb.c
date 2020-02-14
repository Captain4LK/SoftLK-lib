/*
(C) 2020 Lukas Holzbeierlein (Captain4LK)

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the autors be held liable for any damages
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

SLK_RGB_sprite *SLK_draw_rgb_get_target()
{
   return target_rgb;
}

void SLK_draw_rgb_set_target(SLK_RGB_sprite *s)
{
   if(s==NULL)
   {
      target_rgb = target_rgb_default;
      return;
   }

   target_rgb = s;
}

void SLK_draw_rgb_set_clear_color(SLK_Color color)
{
   target_rgb_clear = color;
}

void SLK_draw_rgb_clear()
{
   for(int i = 0;i<target_rgb->width*target_rgb->height;i++)
      target_rgb->data[i] = target_rgb_clear;
}

void SLK_draw_rgb_color(int x, int y, SLK_Color color)
{
   if(color.a&&INBOUNDS(0,target_rgb->width,x)&&INBOUNDS(0,target_rgb->height,y))
      target_rgb->data[y*target_rgb->width+x] = color;
}
      
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
						if(!text_sprite_pal->data[(y_+oy*8)*128+x_+ox*8].mask)
							for(int is = 0;is<scale;is++)
								for(int js = 0;js<scale;js++)
									SLK_draw_rgb_color(x+sx+(x_*scale)+is,y+sy+(y_*scale)+js,color);
			}
			else
			{
				for(int x_ = 0; x_ < 8; x_++)
					for(int y_ = 0; y_ < 8; y_++)
						if(!text_sprite_pal->data[(y_+oy*8)*128+x_+ox*8].mask)
						 	SLK_draw_rgb_color(x+sx+x_,y+sy+y_,color);
			}
			sx += 8*scale;
		}
	}
}

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
   if(x+draw_end_x>target_pal->width)
      draw_end_x = s->width+(target_pal->width-x-draw_end_x);
   if(y+draw_end_y>target_pal->height)
      draw_end_y = s->height+(target_pal->height-y-draw_end_y);
    
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

void SLK_draw_rgb_vertical_line(int x, int y0, int y1, SLK_Color color)
{
   for(int y = y0;y<y1;y++)
      SLK_draw_rgb_color(x,y,color);
}

void SLK_draw_rgb_horizontal_line(int x0, int x1, int y, SLK_Color color)
{
   for(int x = x0;x<x1;x++)
      SLK_draw_rgb_color(x,y,color);
}

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

void SLK_draw_rgb_fill_rectangle(int x, int y, int width, int height, SLK_Color color)
{
   for(int x_ = x;x_<x+width;x_++)
      for(int y_ = y;y_<y+height;y_++)
         SLK_draw_rgb_color(x_,y_,color);
}

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
