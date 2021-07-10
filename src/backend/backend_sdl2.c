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
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
//-------------------------------------

//Internal includes
#include "../../include/SLK/SLK_types.h"
#include "../../include/SLK/SLK_functions.h"
#include "../SLK_layer_i.h"
#include "../backend.h"

#if SLK_ENABLE_RGB
#define CUTE_PNG_IMPLEMENTATION
#define CUTE_PNG_ALLOC backend_malloc
#define CUTE_PNG_FREE backend_free
#include "../../external/cute_png.h"
//https://github.com/RandyGaul/cute_headers
#endif

#define HLH_SLK_IMPLEMENTATION
#define HLH_SLK_MALLOC backend_malloc
#define HLH_SLK_FREE backend_free
#include "../../external/HLH_slk.h"
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
static SDL_Renderer *renderer;
static SDL_Texture **layer_textures;
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
static int text_input_max;
static int mouse_x;
static int mouse_y;
static int mouse_wheel;

static void *(*bmalloc)(size_t size) = backend_system_malloc;
static void (*bfree)(void *ptr) = backend_system_free;
static void *(*brealloc)(void *ptr, size_t size) = backend_system_realloc;
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

   if(layer_dynamic)
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

   SDL_Rect v;
   v.x = view_x;
   v.y = view_y;
   v.w = view_width;
   v.h = view_height;
   if(SDL_RenderSetViewport(renderer,&v)<0)
      SLK_warning("failed to set render viewport: %s",SDL_GetError());
}

//Handles window and input events.
void backend_handle_events()
{
   mouse_wheel = 0;
   memcpy(old_key_state,new_key_state,sizeof(new_key_state));
   memcpy(old_mouse_state,new_mouse_state,sizeof(new_mouse_state));
   for(int i = 0;i<MAX_CONTROLLERS;i++)
      memcpy(gamepads[i].old_button_state,gamepads[i].new_button_state,sizeof(gamepads[0].new_button_state));

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
         if(text_input_active&&strlen(text_input)+strlen(event.text.text)<text_input_max)
            strcat(text_input,event.text.text);
         break;
      case SDL_MOUSEWHEEL:
         mouse_wheel = event.wheel.y;
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
         if(event.window.event==SDL_WINDOWEVENT_RESIZED)
         {
            if(layer_dynamic)
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
   mouse_x = x/pixel_scale;
   mouse_y = y/pixel_scale;

   SDL_GetRelativeMouseState(&mouse_x_rel,&mouse_y_rel);
   mouse_x_rel = mouse_x_rel/pixel_scale;
   mouse_y_rel = mouse_y_rel/pixel_scale;

   if(mouse_x>=screen_width)
     mouse_x = screen_width-1;
   if(mouse_y>=screen_height)
     mouse_y = screen_height-1;

   if(mouse_x<0)
     mouse_x = 0;
   if(mouse_y<0)
     mouse_y = 0;
}

//Creates the window, etc.
void backend_setup(int width, int height, int layer_num, const char *title, int fullscreen, int scale, int resizable)
{
   pixel_scale = scale;
   screen_width = width;
   screen_height = height;
   layer_count = layer_num;
   layer_dynamic = resizable;

   Uint32 flags = 
#ifndef __EMSCRIPTEN__
   SDL_INIT_EVERYTHING;
#else
   SDL_INIT_VIDEO|SDL_INIT_EVENTS;
#endif

   if(SDL_Init(flags)<0)
   {
      SLK_error("failed to init sdl: %s",SDL_GetError());
      exit(-1);
   }

   if(pixel_scale==SLK_WINDOW_MAX)
   {
      SDL_Rect max_size;

      if(SDL_GetDisplayUsableBounds(0,&max_size)<0)
      {
         SLK_warning("failed to get max dimensions: %s",SDL_GetError());
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

   if(resizable)
      sdl_window = SDL_CreateWindow(title,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,width*pixel_scale,height*pixel_scale,SDL_WINDOW_RESIZABLE);
   else
      sdl_window = SDL_CreateWindow(title,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,width*pixel_scale,height*pixel_scale,0);

   if(sdl_window==NULL)
   {
      SLK_error("failed to create window: %s",SDL_GetError());
      exit(-1);
   }

   renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
   if(!renderer)
   {
      SLK_error("failed to create renderer: %s",SDL_GetError());
      exit(-1);
   }

   SDL_SetRenderDrawColor(renderer,0,0,0,0);

   layer_textures = backend_malloc(sizeof(*layer_textures)*layer_num);
   if(layer_textures==NULL)
      SLK_error("malloc of size %zu failed, out of memory!",sizeof(*layer_textures)*layer_num);

   memset(layer_textures,0,sizeof(*layer_textures)*layer_num);
   backend_update_viewport();
}

//Clears the window and redraws the scene.
//Drawing is performed from back to front, layer 0 is always drawn last.
void backend_render_update()
{
   SDL_RenderClear(renderer);

   for(int l = layer_count-1;l>=0;l--)
   {
      layers[l].resized = 0;

      if(layers[l].active)
      {
         switch(layers[l].type)
         {
         case SLK_LAYER_PAL:
         {
#if SLK_ENABLE_PAL
            float width = (float)layers[l].type_0.target->width*layers[l].scale*pixel_scale;
            float height = (float)layers[l].type_0.target->height*layers[l].scale*pixel_scale;
            float x = (float)layers[l].x*pixel_scale;
            float y = (float)layers[l].y*pixel_scale;
            SDL_Rect dst_rect;
            dst_rect.x = x;
            dst_rect.y = y;
            dst_rect.w = width;
            dst_rect.h = height;

            for(int i = 0;i<layers[l].type_0.render->width*layers[l].type_0.render->height;i++)
               layers[l].type_0.render->data[i] = layers[l].type_0.palette->colors[layers[l].type_0.target->data[i]];

            int w, h;
            SDL_QueryTexture(layer_textures[l], NULL, NULL, &w, &h);

            if(w!=layers[l].type_0.target->width||h!=layers[l].type_0.target->height)
            {
               SDL_DestroyTexture(layer_textures[l]);
               layer_textures[l] = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_STREAMING,layers[l].type_0.target->width,layers[l].type_0.target->height);
               SDL_SetTextureBlendMode(layer_textures[l],SDL_BLENDMODE_BLEND);
            }

            void *data;
            int stride;
            SDL_LockTexture(layer_textures[l],NULL,&data,&stride);
            memcpy(data,layers[l].type_0.render->data,sizeof(*layers[l].type_0.render->data)*layers[l].type_0.target->width*layers[l].type_0.target->height);
            SDL_UnlockTexture(layer_textures[l]);

            SDL_SetTextureColorMod(layer_textures[l],layers[l].tint.r,layers[l].tint.g,layers[l].tint.b);
            SDL_SetTextureAlphaMod(layer_textures[l],layers[l].tint.a);
            SDL_RenderCopy(renderer,layer_textures[l],NULL,&dst_rect);

#endif
            break;
         }
         case SLK_LAYER_RGB:
         {
#if SLK_ENABLE_RGB
            int width = (float)layers[l].type_1.target->width*layers[l].scale*pixel_scale;
            int height = (float)layers[l].type_1.target->height*layers[l].scale*pixel_scale;
            int x = (float)layers[l].x*pixel_scale;
            int y = (float)layers[l].y*pixel_scale;
            SDL_Rect dst_rect;
            dst_rect.x = x;
            dst_rect.y = y;
            dst_rect.w = width;
            dst_rect.h = height;

            if(layers[l].type_1.target->changed)
            {
               int w, h;
               SDL_QueryTexture(layer_textures[l], NULL, NULL, &w, &h);

               if(w!=layers[l].type_1.target->width||h!=layers[l].type_1.target->height)
               {
                  SDL_DestroyTexture(layer_textures[l]);
                  layer_textures[l] = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_STREAMING,layers[l].type_1.target->width,layers[l].type_1.target->height);
                  SDL_SetTextureBlendMode(layer_textures[l],SDL_BLENDMODE_BLEND);
               }
               void *data;
               int stride;
               SDL_LockTexture(layer_textures[l],NULL,&data,&stride);
               memcpy(data,layers[l].type_1.target->data,sizeof(*layers[l].type_1.target->data)*layers[l].type_1.target->width*layers[l].type_1.target->height);
               SDL_UnlockTexture(layer_textures[l]);
               layers[l].type_1.target->changed = 0;
            }
            SDL_SetTextureColorMod(layer_textures[l],layers[l].tint.r,layers[l].tint.g,layers[l].tint.b);
            SDL_SetTextureAlphaMod(layer_textures[l],layers[l].tint.a);
            SDL_RenderCopy(renderer,layer_textures[l],NULL,&dst_rect);

#endif
            break;
         }
         }
      }
   }
   
   SDL_RenderPresent(renderer);
}

void backend_create_layer(unsigned index, int type)
{
   if(index>=layer_count)
      return;

   switch(type)
   {
   case SLK_LAYER_PAL:
#if SLK_ENABLE_PAL
      layer_textures[index] = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_STREAMING,screen_width,screen_height);
      if(layer_textures[index]==NULL)
         SLK_error("failed to create texture for layer %d: %s",index,SDL_GetError());

      if(SDL_SetTextureBlendMode(layer_textures[index],SDL_BLENDMODE_BLEND)<0)
         SLK_warning("failed to set texture blend mode: %s",SDL_GetError());
#endif
      break;
   case SLK_LAYER_RGB:
#if SLK_ENABLE_RGB
      layer_textures[index] = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_STREAMING,screen_width,screen_height);
      if(layer_textures[index]==NULL)
         SLK_error("failed to create texture for layer %d: %s",index,SDL_GetError());

      if(SDL_SetTextureBlendMode(layer_textures[index],SDL_BLENDMODE_BLEND)<0)
         SLK_warning("failed to set texture blend mode: %s",SDL_GetError());
#endif
      break;
   }
}
//-------------------------------------

#undef MAX_CONTROLLERS 
