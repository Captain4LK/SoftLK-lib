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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../../external/stb_image.h"
#define CUTE_PNG_IMPLEMENTATION
#include "../../external/cute_png.h"
//https://github.com/nothings/stb
//-------------------------------------

//Internal includes
//-------------------------------------

//#defines
#define MIN(a,b) \
   (((a^b)&-(a<=b))^b)
//-------------------------------------

//Typedefs
typedef struct
{
   struct
   {
      uint8_t r;
      uint8_t g;
      uint8_t b;
      uint8_t a;
   };
}Pixel;

typedef struct
{
   int32_t width;
   int32_t height;
   Pixel *data;
}Image_rgb;

typedef struct
{
   uint8_t index;
   uint8_t mask;
}Paxel;

typedef struct
{
   int32_t width;
   int32_t height;
   Paxel *data;
}Image_pal;

typedef struct
{
   Pixel colors[256];
   int used;
}Palette;
//-------------------------------------

//Variables
static char **input;
static char *output;
static int input_used = 0;
static int mode = 0;
static int save_format = 0;

static uint8_t dither_tresshold_normal[64] = 
{
   0 ,48,12,60, 3,51,15,63,
   32,16,44,28,35,19,47,31,
   8 ,56, 4,52,11,59, 7,55,
   40,24,36,20,43,27,39,23, 
   2 ,50,14,62, 1,49,13,61, 
   34,18,46,30,33,17,45,29, 
   10,58, 6,54, 9,57, 5,53, 
   42,26,38,22,41,25,37,21
};

static uint8_t dither_tresshold_some[64] = 
{
   0 ,24, 6,30, 1,26, 7,32,
   16, 8,22,14,17,10,24,15,
   4 ,28, 2,26, 5,29, 4,28,
   20,12,18,10,21,14,19,11, 
   1 ,25, 7,31, 1,25, 7,30, 
   17, 9,23,15,16, 8,22,15, 
    5,29, 3,27, 4,28, 3,26, 
   21,13,19,11,21,12,18,10
};

static uint8_t dither_tresshold_none[64] = 
{
   0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0
};

static uint8_t *dither_tresshold;
static Palette pal;
static int palette = -1; //-1 --> rgb332, 0 --> load from file
//-------------------------------------

//Function prototypes
static Pixel dither_xy(int x, int y, Pixel c);
static inline uint8_t closest_rg(uint8_t c);
static inline uint8_t closest_b(uint8_t c);
static void palette_load(const char *path); 
static int color_dist2(Pixel c0, Pixel c1);
static Pixel find_closest(Pixel in);
static uint8_t find_palette(Pixel in);
//-------------------------------------

//Function implementations

int main(int argc, char **argv)
{
   //Allocate enough memory for unreachable worst-case szenario.
   input = malloc(sizeof(char *)*argc);
   dither_tresshold = dither_tresshold_normal;

   for(int i = 1;i<argc;i++)
   {
      if(argv[i][0]=='-')  //eg -h, -i etc
      {
         switch(argv[i][1])
         {
         case 'h':
            printf("SoftLK-converter\nCopyright (C) 2020 Captain4LK (Lukas Holzbeierlein)\nUsage: slk_converter [OPTION]...\nConverts PNGs (and other image formats) to the file format used by SoftLK-lib\n\t-h\t\tshow this text\n" \
               "\t-i [FILES]\tspecifies the input files\n\t-o [DIR]\tspecifies the output directory\n\t-f [FORMAT]\timage format to output, either png or slk\n\t-d [LEVEL]\tdithering level, NONE,SOME,NORMAL\n\t-p [PATH]\tpalette to use, rgb332 by default\n");
            exit(0);
            break;
         case 'i':
            mode = 1;
            break;
         case 'o':
            mode = 2;
            break;
         case 'f':
            mode = 3;
            break;
         case 'd':
            mode = 4;
            break;
         case 'p':
            mode = 5;
            break;
         default:
            printf("Unknown argument, try slk_converter -h for help\n");
            break;
         }
      }
      else
      {
         switch(mode)
         {
         case 1: //Add image to convert
            input[input_used++] = argv[i];
            break;
         case 2: //Set output directory
            output = argv[i];
            break;
         case 3: //Set output format
            if(strcmp(argv[i],"slk")==0)
               save_format = 1;
            else if(strcmp(argv[i],"png")==0)
               save_format = 0;
            else
               printf("Error: only png and slk formats are currently supported\n");
            break;
         case 4: //Set dithering
            if(strcmp(argv[i],"NONE")==0)
               dither_tresshold = dither_tresshold_none;
            else if(strcmp(argv[i],"SOME")==0)
               dither_tresshold = dither_tresshold_some;
            else if(strcmp(argv[i],"NORMAL")==0)
               dither_tresshold = dither_tresshold_normal;
            else
               printf("Error only NONE SOME NORMAL dithering modes are supported\n");
            break;
         case 5:
            palette_load(argv[i]);
            palette = 0;
            break;
         }
      }
   }

   if(input_used==0)
   {
      printf("No input files specified! Try slk_converter -h for help\n");
      return 0;
   }

   printf("Found %d images\n",input_used);

   //Process individual images
   for(int i = 0;i<input_used;i++)
   {
      printf("converting %s...\n",input[i]);
      Image_rgb *image = malloc(sizeof(Image_rgb));
      unsigned char *data;
      
      data = stbi_load(input[i],&image->width,&image->height,NULL,4);
      image->data = malloc(sizeof(Pixel)*image->width*image->height);
      memcpy(image->data,data,image->width*image->height*sizeof(Pixel));
      stbi_image_free(data);

      for(int y = 0;y<image->height;y++)
      {
         for(int x = 0;x<image->width;x++)
         {
            image->data[y*image->width+x] = dither_xy(x,y,image->data[y*image->width+x]);
         }
      }

      char out_name[256];
      if(save_format==0)
      {
         sprintf(out_name,"%s%d.png",output,i);
         cp_image_t img;
         img.w = image->width;
         img.h = image->height;
         img.pix = (cp_pixel_t *)image->data;
         FILE *f = fopen(out_name,"wb");
         cp_save_png(f,&img);
         fclose(f);
      }
      else //Save as SLKIMAGE file
      {
         sprintf(out_name,"%s%d.slk",output,i);
         Image_pal *out = malloc(sizeof(Image_pal));
         out->width = image->width;
         out->height = image->height;
         out->data = malloc(sizeof(Paxel)*out->width*out->height);

         for(int p = 0;p<out->width*out->height;p++)
         {
            out->data[p].mask = image->data[p].a==0?255:0;
            if(palette==-1)
               out->data[p].index = (image->data[p].r>>5)+(image->data[p].g>>5<<3)+(image->data[p].b>>6<<6);
            else
               out->data[p].index = find_palette(image->data[p]);
         }

         FILE *f = fopen(out_name,"w");
         fprintf(f,"SLKIMAGE");
         fwrite(&out->width,sizeof(int32_t),1,f);
         fwrite(&out->height,sizeof(int32_t),1,f);
         fwrite(out->data,sizeof(Paxel),out->width*out->height,f);
         fclose(f);
         free(out->data);
         free(out);
      }

      free(image->data);
      free(image);
   }

   return 0;
}

static Pixel dither_xy(int x, int y, Pixel c)
{
   uint8_t tresshold_id = ((y & 7) << 3) + (x & 7);
   Pixel out; 
   out.a = c.a;

   if(palette==-1)
   {
      out.r = closest_rg(MIN((c.r+dither_tresshold[tresshold_id]),0xff));
      out.g = closest_rg(MIN((c.g+dither_tresshold[tresshold_id]),0xff));
      out.b = closest_b(MIN((c.b+dither_tresshold[tresshold_id]),0xff));
   }
   else
   {
      Pixel in;
      in.r = MIN((c.r+dither_tresshold[tresshold_id]),0xff);
      in.g = MIN((c.g+dither_tresshold[tresshold_id]),0xff);
      in.b = MIN((c.b+dither_tresshold[tresshold_id]),0xff);
      in.a = c.a;
      out = find_closest(in);
   }

   return out;
}

static inline uint8_t closest_rg(uint8_t c)
{
   return (c>>5<<5);
}

static inline uint8_t closest_b(uint8_t c)
{
   return (c>>6<<6);
}

static void palette_load(const char *path)
{
   char buffer[512];
   int colors = 0;
   uint8_t r,g,b,a;

   FILE *f = fopen(path,"r");
   if(!f)
   {
      printf("Unable to load palette\n");
      exit(-1);
   }

   memset(&pal,0,sizeof(pal));
   for(int i = 0;i<259&&fgets(buffer,512,f);i++)
   {
      if(i==2)
      {
         sscanf(buffer,"%d",&(pal.used));
      }
      else if(i>2&&buffer[0]!='\0')
      {
         if(sscanf(buffer,"%hhu %hhu %hhu %hhu",&r,&g,&b,&a)!=4)
         {
            sscanf(buffer,"%hhu %hhu %hhu",&r,&g,&b);
            a = 255;
         }

         pal.colors[colors].r = r;
         pal.colors[colors].g = g;
         pal.colors[colors].b = b;
         pal.colors[colors].a = a;
         colors++;
      }
   }
}

static int color_dist2(Pixel c0, Pixel c1)
{
   int diff_r = c1.r-c0.r;
   int diff_g = c1.g-c0.g;
   int diff_b = c1.b-c0.b;

   return (diff_r*diff_r+diff_g*diff_g+diff_b*diff_b);
}

static Pixel find_closest(Pixel in)
{
   if(in.a==0)
      return pal.colors[0];

   int min_dist = INT_MAX;
   int min_index = 0;

   for(int i = 0;i<pal.used;i++)
   {   
      int dist = color_dist2(in,pal.colors[i]);
      if(dist<min_dist)
      {
         min_dist = dist;
         min_index = i;
      }
   }

   return pal.colors[min_index];
}

static uint8_t find_palette(Pixel in)
{
   for(uint8_t i = 0;i<pal.used;i++)
   {
      if(pal.colors[i].r==in.r&&pal.colors[i].g==in.g&&pal.colors[i].b==in.b)
         return i;
   }

   return 0;
}
//-------------------------------------
