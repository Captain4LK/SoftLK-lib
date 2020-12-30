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
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "../../external/UtilityLK/include/ULK_slk.h"
//-------------------------------------

//Internal includes
#include "../../include/SLK/SLK_types.h"
#include "../../include/SLK/SLK_functions.h"
#include "../SLK_layer_i.h"
#include "../backend.h"

#define CUTE_PNG_IMPLEMENTATION
#include "../../external/cute_png.h"
//https://github.com/RandyGaul/cute_headers
//-------------------------------------

//#defines
#define MAX_CONTROLLERS 4
//-------------------------------------

//Typedefs
typedef struct
{
   SDL_GameController *gamepad;
   SDL_Haptic *haptic;
   SDL_JoystickID id;
   int connected;
   uint8_t new_button_state[SLK_PAD_MAX];
   uint8_t old_button_state[SLK_PAD_MAX];
}Gamepad;
//-------------------------------------

//Variables
static SDL_Window *sdl_window;
static SDL_GLContext sdl_gl_context;
static int screen_width;
static int screen_height;
static int pixel_scale;
static int window_width;
static int window_height;
static int view_x;
static int view_y;
static int view_width;
static int view_height;
static int fps;
static int frametime;
static int framedelay;
static int framestart;
static float delta;
static GLuint *layer_textures;
static uint8_t key_map[SDL_NUM_SCANCODES];
static uint8_t mouse_map[6];
static uint8_t gamepad_map[SDL_CONTROLLER_BUTTON_MAX];
static uint8_t new_key_state[SLK_KEY_MAX];
static uint8_t old_key_state[SLK_KEY_MAX];
static uint8_t new_mouse_state[SLK_BUTTON_MAX];
static uint8_t old_mouse_state[SLK_BUTTON_MAX];
static Gamepad gamepads[MAX_CONTROLLERS];
static int mouse_x_rel;
static int mouse_y_rel;
static char *text_input;
static int text_input_active;
static int mouse_x;
static int mouse_y;
static int mouse_wheel;
//-------------------------------------

//Function prototypes
static int get_gamepad_index(int which);
//-------------------------------------

//Function implementations

//Shared implementations:
#include "sdl2/sdl2_shared.c"

//(should) center the viewport.
void backend_update_viewport()
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

      view_x = (window_width-view_width)/2;
      view_y = (window_height-view_height)/2;
   }

   glViewport(view_x,view_y,view_width,view_height);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0,screen_width,screen_height,0,1.0,-1.0);
}

//Handles window and input events.
void backend_handle_events()
{
   mouse_wheel = 0;
   memcpy(old_key_state,new_key_state,sizeof(new_key_state));
   memcpy(old_mouse_state,new_mouse_state,sizeof(new_mouse_state));
   for(int i = 0;i<MAX_CONTROLLERS;i++)
      memcpy(gamepads[i].old_button_state,gamepads[i].new_button_state,sizeof(gamepads[0].new_button_state));

   mouse_x_rel = 0;
   mouse_y_rel = 0;

   //Event managing
   SDL_Event event;
   while(SDL_PollEvent(&event))
   {
      switch(event.type)
      {
      case SDL_QUIT:
         SLK_core_quit();
         break;
      case SDL_KEYDOWN:
         if(text_input_active&&event.key.keysym.sym==SDLK_BACKSPACE&&text_input[0]!='\0')
            text_input[strlen(text_input)-1] = '\0';
         if(event.key.state==SDL_PRESSED)
            new_key_state[key_map[event.key.keysym.scancode]] = 1;
         break;
      case SDL_KEYUP:
         if(event.key.state==SDL_RELEASED)
            new_key_state[key_map[event.key.keysym.scancode]] = 0;
         break;
      case SDL_MOUSEBUTTONDOWN:
         if(event.button.state==SDL_PRESSED)
            new_mouse_state[mouse_map[event.button.button]] = 1;
         break;
      case SDL_MOUSEBUTTONUP:
         if(event.button.state==SDL_RELEASED)
            new_mouse_state[mouse_map[event.button.button]] = 0;
         break;       
      case SDL_TEXTINPUT:
         if(text_input_active)
            strcat(text_input,event.text.text);
         break;
      case SDL_MOUSEWHEEL:
         mouse_wheel = event.wheel.y;
         break;
      case SDL_MOUSEMOTION:
         mouse_x_rel+=event.motion.xrel;
         mouse_y_rel+=event.motion.yrel;
         break;
      case SDL_CONTROLLERBUTTONDOWN:
         if(event.cbutton.state==SDL_PRESSED)
         {
            int id = get_gamepad_index(event.cbutton.which);
            gamepads[id].new_button_state[gamepad_map[event.cbutton.button]] = 1;
         }
         break;
      case SDL_CONTROLLERBUTTONUP:
         if(event.cbutton.state==SDL_RELEASED)
         {
            int id = get_gamepad_index(event.cbutton.which);
            gamepads[id].new_button_state[gamepad_map[event.cbutton.button]] = 0;
         }
         break;
      case SDL_CONTROLLERDEVICEADDED:
         {
            int which = event.cdevice.which;
            if(which<MAX_CONTROLLERS)
            {
               gamepads[which].gamepad = SDL_GameControllerOpen(which);
               gamepads[which].connected = 1;
               SDL_Joystick *j = SDL_GameControllerGetJoystick(gamepads[which].gamepad);
               gamepads[which].id = SDL_JoystickInstanceID(j);
            }
         }
         break;
      case SDL_CONTROLLERDEVICEREMOVED:
         {
            int which = event.cdevice.which;
            if(which<0)
               break;
            int id = get_gamepad_index(which);
            gamepads[id].connected = 0;
            SDL_GameControllerClose(gamepads[id].gamepad);
         }
         break;
      case SDL_WINDOWEVENT:
         if(event.window.event==SDL_WINDOWEVENT_RESIZED&&dynamic)
         {
            int new_width = event.window.data1/pixel_scale+1;
            int new_height = event.window.data2/pixel_scale+1;
            screen_width = new_width;
            screen_height = new_height;

            for(int l = 0;l<layer_count;l++)
            {
               if(layers[l].dynamic)
                  SLK_layer_set_size(l,new_width,new_height);
            }

         }
         backend_update_viewport();
         break;
      }
   }
   //-------------------------------------------
   
   int x,y;
   SDL_GetMouseState(&x,&y);

   x-=view_x;
   y-=view_y;
   mouse_x = (int)(((float)x/(float)(window_width-(view_x*2))*(float)screen_width));
   mouse_y = (int)(((float)y/(float)(window_height-(view_y*2))*(float)screen_height));

   if(mouse_x>=screen_width)
     mouse_x= screen_width-1;
   if(mouse_y>=screen_height)
     mouse_y= screen_height-1;

   if(mouse_x<0)
     mouse_x= 0;
   if(mouse_y<1)
     mouse_y= 1;
}

//Creates the window, etc.
void backend_setup(int width, int height, int layer_num, const char *title, int fullscreen, int scale, int resizable)
{
   pixel_scale = scale;
   screen_width = width;
   screen_height = height;
   layer_count = layer_num;
   dynamic = resizable;

   if(SDL_Init(SDL_INIT_EVERYTHING)<0)
   {
      printf("FATAL ERROR: failed to init sdl\n");
      exit(-1);
   }

   if(pixel_scale==SLK_WINDOW_MAX)
   {
      SDL_Rect max_size;

      if(SDL_GetDisplayUsableBounds(0,&max_size)<0)
      {
         printf("Failed to get max dimensions: %s\n",SDL_GetError());
      }
      else
      {
         int max_x,max_y;

         max_x = max_size.w/screen_width;
         max_y = max_size.h/screen_height;

         pixel_scale = (max_x>max_y)?max_y:max_x;
      }
      
   }

   if(pixel_scale<=0)
      pixel_scale = 1;

   SDL_GL_LoadLibrary(NULL);
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,1); 
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,1);
   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

   if(resizable)
      sdl_window = SDL_CreateWindow(title,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,width*pixel_scale,height*pixel_scale,SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
   else
      sdl_window = SDL_CreateWindow(title,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,width*pixel_scale,height*pixel_scale,SDL_WINDOW_OPENGL);

   if(!sdl_window)
   {
      printf("FATAL ERROR: failed to create window\n");
      exit(-1);
   }
    
   sdl_gl_context = SDL_GL_CreateContext(sdl_window);
   if(!sdl_gl_context)
   {
      printf("FATAL ERROR: failed to create opengl context\n");
      exit(-1);
   }
   SDL_GL_SetSwapInterval(0);

   printf("OpenGL loaded\n");
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

   layer_textures = malloc(sizeof(*layer_textures)*layer_num);
   memset(layer_textures,0,sizeof(*layer_textures)*layer_num);
}

//Clears the window and redraws the scene.
//Drawing is performed from back to front, layer 0 is always drawn last.
void backend_render_update()
{
   glClear(GL_COLOR_BUFFER_BIT);
   //glViewport(view_x,view_y,view_width,view_height);

   for(int l = layer_count-1;l>=0;l--)
   {
      layers[l].resized = 0;

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

            glBindTexture(GL_TEXTURE_2D,layer_textures[l]);
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

            glBindTexture(GL_TEXTURE_2D,layer_textures[l]);
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

void backend_create_layer(unsigned index, int type)
{
   if(index>=layer_count)
      return;

   switch(type)
   {
   case SLK_LAYER_PAL:
      glGenTextures(1,&layer_textures[index]);
      glBindTexture(GL_TEXTURE_2D,layer_textures[index]);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
      glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
      glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,screen_width,screen_height,
                   0,GL_RGBA,GL_UNSIGNED_BYTE,layers[index].type_0.render->data);
      break;
   case SLK_LAYER_RGB:
      glGenTextures(1,&layer_textures[index]);
      glBindTexture(GL_TEXTURE_2D,layer_textures[index]);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
      glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
      glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,screen_width,screen_height,
                   0,GL_RGBA,GL_UNSIGNED_BYTE,layers[index].type_1.target->data);
      break;
   }
}
//-------------------------------------

#undef MAX_CONTROLLERS 
