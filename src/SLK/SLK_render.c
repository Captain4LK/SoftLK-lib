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
#include <SDL2/SDL.h>
#include "../../include/glad/glad.h"
//-------------------------------------

//Internal includes
#include "../../include/SLK/SLK_types.h"
#include "../../include/SLK/SLK_functions.h"
#include "SLK_variables.h"
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

//Performs some necessary OpenGL function calls.
//Gets called by SLK_setup, don't call by yourself.
void SLK_render_init()
{
   printf("OpenGL loaded\n");
   gladLoadGLLoader(SDL_GL_GetProcAddress);
   printf("Vendor:   %s\n",glGetString(GL_VENDOR));
   printf("Renderer: %s\n",glGetString(GL_RENDERER));
   printf("Version:  %s\n",glGetString(GL_VERSION));

   glEnable(GL_TEXTURE_2D);
   glViewport(0,0,screen_width,screen_height);
   glClearColor(0.0f,0.0f,0.0f,0.0f);
   glClear(GL_COLOR_BUFFER_BIT);

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

//Clears the window and redraws the scene.
//Drawing is performed from back to front, layer 0 is always drawn last.
void SLK_render_update()
{
   glClear(GL_COLOR_BUFFER_BIT);
   //glViewport(view_x,view_y,view_width,view_height);

   for(int l = layer_count-1;l>=0;l--)
   {
      if(layers[l].active)
      {
         switch(layers[l].type)
         {
         case SLK_LAYER_PAL:
         {
            float width = (float)layers[l].type_0.target->width*layers[l].scale;
            float height = (float)layers[l].type_0.target->height*layers[l].scale;
            float x = (float)layers[l].x;
            float y = (float)layers[l].y;

            for(int i = 0;i<layers[l].type_0.render->width*layers[l].type_0.render->height;i++)
               layers[l].type_0.render->data[i] = layers[l].type_0.palette->colors[layers[l].type_0.target->data[i].index];

            glBindTexture(GL_TEXTURE_2D,layers[l].type_0.texture);
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,layers[l].type_0.render->width,layers[l].type_0.render->height,0,GL_RGBA,GL_UNSIGNED_BYTE,layers[l].type_0.render->data);

            glBegin(GL_QUADS);
               glColor4ub(layers[l].tint.r,layers[l].tint.g,layers[l].tint.b,layers[l].tint.a);
               glTexCoord2i(0,0);
               glVertex3f(x,y,0.0f);
               glTexCoord2i(0,1);
               glVertex3f(x,y+height,0.0f);
               glTexCoord2f(1,1);
               glVertex3f(width+x,y+height,0.0f);
               glTexCoord2f(1,0);
               glVertex3f(width+x,y,0.0f);
            glEnd();

            break;
         }
         case SLK_LAYER_RGB:
         {
            float width = (float)layers[l].type_1.target->width*layers[l].scale;
            float height = (float)layers[l].type_1.target->height*layers[l].scale;
            float x = (float)layers[l].x;
            float y = (float)layers[l].y;

            glBindTexture(GL_TEXTURE_2D,layers[l].type_1.texture);
            if(layers[l].type_1.target->changed)
            {
               glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,layers[l].type_1.target->width,layers[l].type_1.target->height,0,GL_RGBA,GL_UNSIGNED_BYTE,layers[l].type_1.target->data);
               layers[l].type_1.target->changed = 0;
            }

            glBegin(GL_QUADS);
               glColor4ub(layers[l].tint.r,layers[l].tint.g,layers[l].tint.b,layers[l].tint.a);
               glTexCoord2i(0,0);
               glVertex3f(x,y,0.0f);
               glTexCoord2i(0,1);
               glVertex3f(x,y+height,0.0f);
               glTexCoord2f(1,1);
               glVertex3f(width+x,y+height,0.0f);
               glTexCoord2f(1,0);
               glVertex3f(width+x,y,0.0f);
            glEnd();

            break;
         }
         }
      }
   }

   SDL_GL_SwapWindow(sdl_window);
}

//Updates the viewport (the space where everything is drawn in)
//using the current window width and screen width.
void SLK_render_update_viewport()
{
   SDL_GetWindowSize(sdl_window,&window_width,&window_height);

   if(dynamic)
   {
      view_width = window_width;
      view_height = window_height;
      view_x = 0;
      view_y = 0;
   }
   else
   {
      view_width = screen_width*pixel_scale;
      view_height = screen_height*pixel_scale;

      /*if(view_height<window_height)
      {
           int p_scale = window_height/screen_height;
           view_width = screen_width*p_scale;
           view_height = screen_height*p_scale;
       }
       else
       {
          int p_scale = window_width/screen_width;
          view_width = screen_width*p_scale;
          view_height = screen_height*p_scale;
       }*/

       view_x = (window_width-view_width)/2;
       view_y = (window_height-view_height)/2;
   }

   glViewport(view_x,view_y,view_width,view_height);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0,screen_width,screen_height,0,1.0,-1.0);
}
//-------------------------------------
