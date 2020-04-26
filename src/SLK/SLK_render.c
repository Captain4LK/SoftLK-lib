/*
   Copyright (c) 2020, Lukas Holzbeierlein (Captain4LK) 
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
       * Redistributions of source code must retain the above copyright
         notice, this list of conditions and the following disclaimer.
       * Redistributions in binary form must reproduce the above copyright
         notice, this list of conditions and the following disclaimer in the
         documentation and/or other materials provided with the distribution.
       * Neither the name of SLK nor the
         names of its contributors may be used to endorse or promote products
         derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL CAPTAIN4LK BE LIABLE FOR ANY
   DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
   ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "../../include/SLK/SLK.h"
#include "../../include/SLK/SLK_functions.h"
#include "SLK_variables.h"

void SLK_render_init()
{
   printf("OpenGL loaded\n");
   gladLoadGLLoader(SDL_GL_GetProcAddress);
   printf("Vendor:   %s\n",glGetString(GL_VENDOR));
   printf("Renderer: %s\n",glGetString(GL_RENDERER));
   printf("Version:  %s\n",glGetString(GL_VERSION));

   //glDisable(GL_DEPTH_TEST);
   //glDisable(GL_CULL_FACE);
   glEnable(GL_TEXTURE_2D);
   glViewport(0,0,screen_width,screen_height);
   glClearColor(0.0f,0.0f,0.0f,0.0f);
   glClear(GL_COLOR_BUFFER_BIT);

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void SLK_render_update()
{
   glClear(GL_COLOR_BUFFER_BIT);
   glViewport(view_x,view_y,view_width,view_height);

   for(int l = layer_count-1;l>=0;l--)
   {
      if(layers[l].active)
      {
         switch(layers[l].type)
         {
         case SLK_LAYER_PAL:
            for(int i = 0;i<layers[l].type_0.render->width*layers[l].type_0.render->height;i++)
               layers[l].type_0.render->data[i] = layers[l].type_0.palette->colors[layers[l].type_0.target->data[i].index];

            glBindTexture(GL_TEXTURE_2D,layers[l].type_0.texture);
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,screen_width,screen_height,0,GL_RGBA,GL_UNSIGNED_BYTE,layers[l].type_0.render->data);

            glBegin(GL_QUADS);
               glColor4ub(layers[l].tint.r,layers[l].tint.g,layers[l].tint.b,layers[l].tint.a);
               glTexCoord2f(0.0, 1.0);
               glVertex3f(-1.0f,-1.0f,0.0f);
               glTexCoord2f(0.0, 0.0);
               glVertex3f(-1.0f,1.0f,0.0f);
               glTexCoord2f(1.0, 0.0);
               glVertex3f( 1.0f,1.0f,0.0f);
               glTexCoord2f(1.0,1.0);
               glVertex3f(1.0f,-1.0f,0.0f);
            glEnd();

            break;
         case SLK_LAYER_RGB:
            glBindTexture(GL_TEXTURE_2D,layers[l].type_1.texture);
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,screen_width,screen_height,0,GL_RGBA,GL_UNSIGNED_BYTE,layers[l].type_1.target->data);

            glBegin(GL_QUADS);
               glColor4ub(layers[l].tint.r,layers[l].tint.g,layers[l].tint.b,layers[l].tint.a);
               glTexCoord2f(0.0, 1.0);
               glVertex3f(-1.0f,-1.0f,0.0f);
               glTexCoord2f(0.0, 0.0);
               glVertex3f(-1.0f,1.0f,0.0f);
               glTexCoord2f(1.0, 0.0);
               glVertex3f( 1.0f,1.0f,0.0f);
               glTexCoord2f(1.0,1.0);
               glVertex3f(1.0f,-1.0f,0.0f);
            glEnd();

            break;
         }
      }
   }

   SDL_GL_SwapWindow(sdl_window);
}
