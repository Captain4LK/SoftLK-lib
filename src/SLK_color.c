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
#include <math.h>
//-------------------------------------

//Internal includes
#include "../include/SLK/SLK_types.h"
#include "../include/SLK/SLK_functions.h"
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

//Creates a SLK_Color struct from 4 unsigned 8bit ints.
SLK_Color SLK_color_create(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
   SLK_Color c;
   c.r = r;
   c.g = g;
   c.b = b;
   c.a = a;
   
   return c;
} 

//Creates a SLK_Color struct from 4 floats.
SLK_Color SLK_color_create_float(float r, float g, float b, float a)
{
   SLK_Color c;
   c.r = r*255.0f;
   c.g = g*255.0f;
   c.b = b*255.0f;
   c.a = a*255.0f;

   return c;
}

//Creates a SLK_Color struct from 4 floats.
//h is expected to be in range [0,360].
SLK_Color SLK_color_create_hsv(float h, float s, float v, float a)
{
   SLK_Color c;
   if(s<=0.0f)
   {
      c.r = 0;
      c.g = 0;
      c.b = 0;
      c.a = a*255.0f;

      return c;
   }

   float ch = s*v;
   if(h>=360.0f)
      h = 0.0f;
   h/=60;
   float x = ch*(1-fabs(fmod(h,2)-1));
   float m = v-ch;
   float r,g,b;

   if(h>=0.0f&&h<=1.0f)
   {
      r = ch;
      g = x;
      b = 0.0f;
   }
   else if(h>1.0f&&h<=2.0f)
   {
      r = x;
      g = ch;
      b = 0.0f;
   }
   else if(h>2.0f&&h<=3.0f)
   {
      r = 0.0f;
      g = ch;
      b = x;
   }
   else if(h>3.0f&&h<=4.0f)
   {
      r = 0.0f;
      g = x;
      b = ch;
   }
   else if(h>4.0f&&h<=5.0f)
   {
      r = x;
      g = 0.0f;
      b = ch;
   }
   else
   {
      r = ch;
      g = 0.0f;
      b = x;
   }

   c.r = (r+m)*255.0f;
   c.g = (g+m)*255.0f;
   c.b = (b+m)*255.0f;
   c.a = a*255.0f;

   return c;
}

//Creates a SLK_Color struct from 4 floats.
//h is expected to be in range [0,360].
SLK_Color SLK_color_create_hsl(float h, float s, float l, float a)
{
   SLK_Color c;
   if(s<=0.0f)
   {
      c.r = 0;
      c.g = 0;
      c.b = 0;
      c.a = a*255.0f;

      return c;
   }

   float ch = (1.0f-fabs(2.0f*l-1.0f))*s;
   if(h>=360.0f)
      h = 0.0f;
   h/=60;
   float x = ch*(1-fabs(fmod(h,2)-1));
   float m = l-(ch/2.0f);
   float r,g,b;

   if(h>=0.0f&&h<=1.0f)
   {
      r = ch;
      g = x;
      b = 0.0f;
   }
   else if(h>1.0f&&h<=2.0f)
   {
      r = x;
      g = ch;
      b = 0.0f;
   }
   else if(h>2.0f&&h<=3.0f)
   {
      r = 0.0f;
      g = ch;
      b = x;
   }
   else if(h>3.0f&&h<=4.0f)
   {
      r = 0.0f;
      g = x;
      b = ch;
   }
   else if(h>4.0f&&h<=5.0f)
   {
      r = x;
      g = 0.0f;
      b = ch;
   }
   else
   {
      r = ch;
      g = 0.0f;
      b = x;
   }

   c.r = (r+m)*255.0f;
   c.g = (g+m)*255.0f;
   c.b = (b+m)*255.0f;
   c.a = a*255.0f;

   return c;
}
//-------------------------------------
