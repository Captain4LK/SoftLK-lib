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
#include "../../include/SLK/SLK_types.h"
#include "../../include/SLK/SLK_functions.h"
#include "SLK_input_i.h"
#include "SLK_render_i.h"
#include "SLK_draw_rgb_i.h"
#include "SLK_draw_pal_i.h"
#include "SLK_layer_i.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
uint8_t key_map[SDL_NUM_SCANCODES];
uint8_t mouse_map[6];
int running;
//-------------------------------------

//Function prototypes
//-------------------------------------

//Function implementations

//Sets the running flag to 0.
void SLK_core_quit()
{
   running = 0;
}

//Checks wether SoftLK is supposed to be running or not.
int SLK_core_running()
{
    return running;
}

//Sets the title of the window.
//I did not test this since I am using
//a window manager without titlebars.
void SLK_core_set_title(const char *title)
{
   SDL_SetWindowTitle(sdl_window,title);
}

void SLK_core_set_fullscreen(int fullscreen)
{
   if(fullscreen)
   {
      SDL_SetWindowFullscreen(sdl_window,SDL_WINDOW_FULLSCREEN_DESKTOP);
   }
   else
   {
      SDL_SetWindowFullscreen(sdl_window,0);
      SDL_SetWindowSize(sdl_window,screen_width*pixel_scale,screen_height*pixel_scale);
   }

   SLK_i_render_update_viewport();
}

//Sets the icon of the window.
//I did not test this since I am using
//a window manager without titlebars.
void SLK_core_set_icon(const SLK_RGB_sprite *icon)
{
   SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(icon->data,icon->width,icon->height,32,icon->width*4,0xf000,0x0f00,0x00f0,0x000f);

   SDL_SetWindowIcon(sdl_window,surface);

   SDL_FreeSurface(surface);
}

//Returns the current viewport width.
int SLK_core_get_width()
{
   return screen_width;
}

//Returns the current viewport height.
int SLK_core_get_height()
{
   return screen_height;
}

//Updates the engine state.
//Updates the input and the timer.
void SLK_update()
{
   SLK_timer_update();

   SLK_i_mouse_update_wheel(0);
   memcpy(old_key_state,new_key_state,sizeof(new_key_state));
   memcpy(old_mouse_state,new_mouse_state,sizeof(new_mouse_state));

   //Event managing
   SDL_Event event;
   while(SDL_PollEvent(&event))
   {
      switch(event.type)
      {
      case SDL_QUIT:
         running = 0;
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
         SLK_i_mouse_update_wheel(event.wheel.y);
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

            SLK_i_render_update_viewport();
         }

         break;
      }
   }
   //-------------------------------------------
   
   int x,y;
   SDL_GetMouseState(&x,&y);
   SLK_i_mouse_update(x,y);
}

//The first function you should call in your code.
//Creates a window, sets its title and allocates space for the layers.
//Also loads the font files in the data dir if availible.
void SLK_setup(const int width, const int height, const int layer_num, const char *title, const int fullscreen, int scale, int resizable)
{
   pixel_scale = scale;
   screen_width = width;
   screen_height = height;
   layer_count = layer_num;
   dynamic = resizable;
   layers = malloc(sizeof(SLK_Layer)*layer_num);
   memset(layers,0,sizeof(SLK_Layer)*layer_num);

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
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,2); 
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

   SLK_i_render_init();
   SLK_core_set_fullscreen(fullscreen);

   text_sprite_pal = SLK_pal_sprite_load("data/font8x8.slk");
   text_sprite_rgb = SLK_rgb_sprite_load("data/font8x8.png");

   key_map[0x00] = SLK_KEY_NONE;
   key_map[SDL_SCANCODE_A] = SLK_KEY_A;
   key_map[SDL_SCANCODE_B] = SLK_KEY_B;
   key_map[SDL_SCANCODE_C] = SLK_KEY_C;
   key_map[SDL_SCANCODE_D] = SLK_KEY_D;
   key_map[SDL_SCANCODE_E] = SLK_KEY_E;
   key_map[SDL_SCANCODE_F] = SLK_KEY_F;
   key_map[SDL_SCANCODE_G] = SLK_KEY_G;
   key_map[SDL_SCANCODE_H] = SLK_KEY_H;
   key_map[SDL_SCANCODE_I] = SLK_KEY_I;
   key_map[SDL_SCANCODE_J] = SLK_KEY_J;
   key_map[SDL_SCANCODE_K] = SLK_KEY_K;
   key_map[SDL_SCANCODE_L] = SLK_KEY_L;
   key_map[SDL_SCANCODE_M] = SLK_KEY_M;
   key_map[SDL_SCANCODE_N] = SLK_KEY_N;
   key_map[SDL_SCANCODE_O] = SLK_KEY_O;
   key_map[SDL_SCANCODE_P] = SLK_KEY_P;
   key_map[SDL_SCANCODE_Q] = SLK_KEY_Q;
   key_map[SDL_SCANCODE_R] = SLK_KEY_R;
   key_map[SDL_SCANCODE_S] = SLK_KEY_S;
   key_map[SDL_SCANCODE_T] = SLK_KEY_T;
   key_map[SDL_SCANCODE_U] = SLK_KEY_U;
   key_map[SDL_SCANCODE_V] = SLK_KEY_V;
   key_map[SDL_SCANCODE_W] = SLK_KEY_W;
   key_map[SDL_SCANCODE_X] = SLK_KEY_X;
   key_map[SDL_SCANCODE_Y] = SLK_KEY_Y;
   key_map[SDL_SCANCODE_Z] = SLK_KEY_Z;

   key_map[SDL_SCANCODE_F1] = SLK_KEY_F1;
   key_map[SDL_SCANCODE_F2] = SLK_KEY_F2;
   key_map[SDL_SCANCODE_F3] = SLK_KEY_F3;
   key_map[SDL_SCANCODE_F4] = SLK_KEY_F4;
   key_map[SDL_SCANCODE_F5] = SLK_KEY_F5;
   key_map[SDL_SCANCODE_F6] = SLK_KEY_F6;
   key_map[SDL_SCANCODE_F7] = SLK_KEY_F7;
   key_map[SDL_SCANCODE_F8] = SLK_KEY_F8;
   key_map[SDL_SCANCODE_F9] = SLK_KEY_F9;
   key_map[SDL_SCANCODE_F10] = SLK_KEY_F10;
   key_map[SDL_SCANCODE_F11] = SLK_KEY_F11;
   key_map[SDL_SCANCODE_F12] = SLK_KEY_F12;

   key_map[SDL_SCANCODE_DOWN] = SLK_KEY_DOWN;
   key_map[SDL_SCANCODE_LEFT] = SLK_KEY_LEFT;
   key_map[SDL_SCANCODE_RIGHT] = SLK_KEY_RIGHT;
   key_map[SDL_SCANCODE_UP] = SLK_KEY_UP;
   key_map[SDL_SCANCODE_RETURN] = SLK_KEY_ENTER;

   key_map[SDL_SCANCODE_BACKSPACE] = SLK_KEY_BACK;
   key_map[SDL_SCANCODE_ESCAPE] = SLK_KEY_ESCAPE;
   key_map[SDL_SCANCODE_TAB] = SLK_KEY_TAB;
   key_map[SDL_SCANCODE_LGUI] = SLK_KEY_HOME;
   /*key_map[XK_End] = SLK_KEY_END;
   key_map[XK_Page_Up] = SLK_KEY_PGUP;
   key_map[XK_Page_Down] = SLK_KEY_PGDN;
   key_map[XK_Insert] = SLK_KEY_INS;
   key_map[XK_Shift_L] = SLK_KEY_SHIFT;
   key_map[XK_Shift_R] = SLK_KEY_SHIFT;*/
   key_map[SDL_SCANCODE_LCTRL] = SLK_KEY_CTRL;
   key_map[SDL_SCANCODE_RCTRL] = SLK_KEY_CTRL;
   key_map[SDL_SCANCODE_SPACE] = SLK_KEY_SPACE;

   /*key_map[XK_0] = SLK_KEY_K0;
   key_map[XK_1] = SLK_KEY_K1;
   key_map[XK_2] = SLK_KEY_K2;
   key_map[XK_3] = SLK_KEY_K3;
   key_map[XK_4] = SLK_KEY_K4;
   key_map[XK_5] = SLK_KEY_K5;
   key_map[XK_6] = SLK_KEY_K6;
   key_map[XK_7] = SLK_KEY_K7;
   key_map[XK_8] = SLK_KEY_K8;
   key_map[XK_9] = SLK_KEY_K9;

   key_map[XK_KP_0] = SLK_KEY_NP0;
   key_map[XK_KP_1] = SLK_KEY_NP1;
   key_map[XK_KP_2] = SLK_KEY_NP2;
   key_map[XK_KP_3] = SLK_KEY_NP3;
   key_map[XK_KP_4] = SLK_KEY_NP4;
   key_map[XK_KP_5] = SLK_KEY_NP5;
   key_map[XK_KP_6] = SLK_KEY_NP6;
   key_map[XK_KP_7] = SLK_KEY_NP7;
   key_map[XK_KP_8] = SLK_KEY_NP8;
   key_map[XK_KP_9] = SLK_KEY_NP9;
   key_map[XK_KP_Multiply] = SLK_KEY_NP_MUL;
   key_map[XK_KP_Add] = SLK_KEY_NP_ADD;
   key_map[XK_KP_Divide] = SLK_KEY_NP_DIV;
   key_map[XK_KP_Subtract] = SLK_KEY_NP_SUB;
   key_map[XK_KP_Decimal] = SLK_KEY_NP_DECIMAL;*/

   mouse_map[SDL_BUTTON_LEFT] = SLK_BUTTON_LEFT;
   mouse_map[SDL_BUTTON_RIGHT] = SLK_BUTTON_RIGHT;
   mouse_map[SDL_BUTTON_MIDDLE] = SLK_BUTTON_MIDDLE;
   mouse_map[SDL_BUTTON_X1] = SLK_BUTTON_X1;
   mouse_map[SDL_BUTTON_X2] = SLK_BUTTON_X2;

   running = 1;
   memset(new_key_state,0,sizeof(new_key_state));
   memset(new_mouse_state,0,sizeof(new_mouse_state));
}
//-------------------------------------
