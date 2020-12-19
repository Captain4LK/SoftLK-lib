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
//-------------------------------------

//Internal includes
#include "../../external/UtilityLK/include/ULK_slk.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
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
//-------------------------------------

//Variables
static char **input;
static char *output;
static int input_used = 0;
static int mode = 0;
static int save_format = 0;
//-------------------------------------

//Function prototypes
static Image_pal *load_pal_file(FILE *f);
//-------------------------------------

//Function implementations

int main(int argc, char **argv)
{
   //Allocate enough memory for unreachable worst-case szenario.
   input = malloc(sizeof(char *)*argc);

   for(int i = 1;i<argc;i++)
   {
      if(argv[i][0]=='-')  //eg -h, -i etc
      {
         switch(argv[i][1])
         {
         case 'h':
            printf("SoftLK-ImageUpdater\nCopyright (C) 2020 Captain4LK (Lukas Holzbeierlein)\nUsage: slk_img_updater [OPTION]...\nConverts .slk images to the new file format used by SoftLK-lib\n\t-h\t\tshow this text\n" \
               "\t-i [FILES]\tspecifies the input files\n\t-o [DIR]\tspecifies the output directory\n\t-r [MODE] set the rle encoding mode, 0 for none, 3 for all, 1 for indices, 2 for masks\n");
            exit(0);
            break;
         case 'i':
            mode = 1;
            break;
         case 'o':
            mode = 2;
            break;
         case 'r':
            mode = 3;
            break;
         default:
            printf("Unknown argument, try slk_img_updater -h for help\n");
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
            if(strcmp(argv[i],"0")==0)
               save_format = 0;
            else if(strcmp(argv[i],"1")==0)
               save_format = 1;
            else if(strcmp(argv[i],"2")==0)
               save_format = 2;
            else if(strcmp(argv[i],"3")==0)
               save_format = 3;
            else
               printf("Error: rle mode %s not supported\n",argv[i]);
            break;
         }
      }
   }

   if(input_used==0)
   {
      printf("No input files specified! Try slk_mg_updater -h for help\n");
      return 0;
   }

   printf("Found %d images\n",input_used);

   //Process individual images
   for(int i = 0;i<input_used;i++)
   {
      FILE *f = fopen(input[i],"rb");
      if(!f)
         continue;
      puts(input[i]);
      Image_pal *in = load_pal_file(f);   
      if(!in)
         continue;
      fclose(f);
      ULK_slk_image out;
      out.width = in->width;
      out.height = in->height;
      out.data = (ULK_slk_paxel *)in->data;
      printf("%d %d\n",in->width,in->height);

      char out_name[256];
      sprintf(out_name,"%s%d.slk",output,i);
      f = fopen(out_name,"wb");
      ULK_slk_image_write(&out,f,save_format);
      fclose(f);

      free(in->data);
      free(in);
   }
 
   return 0;
}

static Image_pal *load_pal_file(FILE *f)
{
   Image_pal *s = NULL;
   int32_t width, height;
   char file_type[512];

   fread(file_type,sizeof(file_type[0]),8,f);
   file_type[8] = '\0';
   if(strcmp(file_type,"SLKIMAGE")!=0)
   {
      puts("File does not seem to be a SLKIMAGE file");
      return NULL;
   }
      
   fread(&width,sizeof(width),1,f);
   fread(&height,sizeof(height),1,f);   
   
   s = malloc(sizeof(*s));
   s->data = malloc(sizeof(*s->data)*width*height);
   s->width = width;
   s->height = height;
   fread(s->data,sizeof(*s->data),width*height,f);

   return s;
}
//-------------------------------------
