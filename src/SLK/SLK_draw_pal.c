/*
(C) 2020 Lukas Holzbeierlein (Captain4LK)

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

SLK_Pal_sprite *SLK_draw_pal_get_target()
{
   return target_pal;
}

void SLK_draw_pal_set_target(SLK_Pal_sprite *s)
{
   if(s==NULL)
   {
      target_pal = target_pal_default;
      return;
   }

   target_pal = s;
}

void SLK_draw_pal_set_clear_paxel(SLK_Paxel paxel)
{
   target_pal_clear = paxel;
}

void SLK_draw_pal_clear()
{
   for(int i = 0;i<target_pal->width*target_pal->height;i++)
      target_pal->data[i] = target_pal_clear;
}

void SLK_draw_pal_paxel(int x, int y, SLK_Paxel paxel)
{
   if(INBOUNDS(0,target_pal->width,x)&&INBOUNDS(0,target_pal->height,y))
   {
      int index = y*target_pal->width+x;
      target_pal->data[index].index = (target_pal->data[index].index&paxel.mask)|paxel.index;
   }
}

void SLK_draw_pal_string(int x, int y, int scale, const char *text, SLK_Paxel paxel)
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
									SLK_draw_pal_paxel(x+sx+(x_*scale)+is,y+sy+(y_*scale)+js,paxel);
			}
			else
			{
				for(int x_ = 0; x_ < 8; x_++)
					for(int y_ = 0; y_ < 8; y_++)
						if(!text_sprite_pal->data[(y_+oy*8)*128+x_+ox*8].mask)
						 	SLK_draw_pal_paxel(x+sx+x_,y+sy+y_,paxel);
			}
			sx += 8*scale;
		}
	}
}

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
    
   for(int x1 = draw_start_x;x1<draw_end_x;x1++)
   {
      for(int y1 = draw_start_y;y1<draw_end_y;y1++)
      {
         SLK_Paxel p = s->data[y1*s->width+x1];
         int index = (y1+y)*target_pal->width+x1+x;
         target_pal->data[index].index = (target_pal->data[index].index&p.mask)|p.index;
      }
   }
}

void SLK_draw_pal_sprite_partial(const SLK_Pal_sprite *s, int x, int y, int ox, int oy, int width, int height)
{
   int draw_start_y = oy;
   int draw_start_x = ox;
   int draw_end_x = ox+width;
   int draw_end_y = oy+height;

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
         SLK_Paxel p = s->data[y1*s->width+x1];
         int index = (y1+y-oy)*target_pal->width+x1+x-ox;
         target_pal->data[index].index = (target_pal->data[index].index&p.mask)|p.index;
      }
   }
}

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

   if(flip==SLK_FLIP_NONE)
   {
      for(int x1 = draw_start_x;x1<draw_end_x;x1++)
      {
         for(int y1 = draw_start_y;y1<draw_end_y;y1++)
         {
            SLK_Paxel p = s->data[y1*s->width+x1];
            int index = (y1+y)*target_pal->width+x1+x;
            target_pal->data[index].index = (target_pal->data[index].index&p.mask)|p.index;
         }
      }
   }
   else if(flip==SLK_FLIP_VERTICAL)
   {
      for(int x1 = draw_start_x;x1<draw_end_x;x1++)
      {
         for(int y1 = draw_start_y;y1<draw_end_y;y1++)
         {
            SLK_Paxel p = s->data[(s->height-y1-1)*s->width+x1];
            int index = (y1+y)*target_pal->width+x1+x;
            target_pal->data[index].index = (target_pal->data[index].index&p.mask)|p.index;
         }
      }
   }
   else if(flip==SLK_FLIP_HORIZONTAL)
   {
      for(int x1 = draw_start_x;x1<draw_end_x;x1++)
      {
         for(int y1 = draw_start_y;y1<draw_end_y;y1++)
         {
            SLK_Paxel p = s->data[y1*s->width+(s->width-x1-1)];
            int index = (y1+y)*target_pal->width+x1+x;
            target_pal->data[index].index = (target_pal->data[index].index&p.mask)|p.index;
         }
      }
   }
   else if(flip==(SLK_FLIP_VERTICAL|SLK_FLIP_HORIZONTAL))
   {
      for(int x1 = draw_start_x;x1<draw_end_x;x1++)
      {
         for(int y1 = draw_start_y;y1<draw_end_y;y1++)
         {
            SLK_Paxel p = s->data[(s->height-y1-1)*s->width+(s->width-x1)];
            int index = (y1+y)*target_pal->width+x1+x;
            target_pal->data[index].index = (target_pal->data[index].index&p.mask)|p.index;
         }
      }
   }
}

void SLK_draw_pal_line(int x0, int y0, int x1, int y1, SLK_Paxel paxel)
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
        SLK_draw_pal_paxel(x,y,paxel);

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

void SLK_draw_pal_vertical_line(int x, int y0, int y1, SLK_Paxel paxel)
{
   for(int y = y0;y<y1;y++)
      SLK_draw_pal_paxel(x,y,paxel);
}

void SLK_draw_pal_horizontal_line(int x0, int x1, int y, SLK_Paxel paxel)
{
   for(int x = x0;x<x1;x++)
      SLK_draw_pal_paxel(x,y,paxel);
}

void SLK_draw_pal_rectangle(int x, int y, int width, int height, SLK_Paxel paxel)
{
   for(int y_ = y;y_<y+height;y_++)
   {
      SLK_draw_pal_paxel(x,y_,paxel);
      SLK_draw_pal_paxel(x+width-1,y_,paxel);
   }

   for(int x_ = x;x_<x+width;x_++)
   {
      SLK_draw_pal_paxel(x_,y,paxel);
      SLK_draw_pal_paxel(x_,y+height-1,paxel);
   }
}

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
