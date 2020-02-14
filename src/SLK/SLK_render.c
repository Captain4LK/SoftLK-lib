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
