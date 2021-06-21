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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "../../external/UtilityLK/include/ULK_slk.h"
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
static SDL_Surface *surface_window;
static SDL_Rect surface_base_rect;
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
static SDL_Surface **layer_surfaces;
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

   surface_base_rect.x = view_x;
   surface_base_rect.y = view_y;
   surface_base_rect.w = view_width;
   surface_base_rect.h = view_height;
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
            surface_window = SDL_GetWindowSurface(sdl_window);
         }
         backend_update_viewport();
         SDL_SetClipRect(surface_window,&surface_base_rect);
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
   mouse_x_rel = (int)(((float)mouse_x_rel/(float)(window_width-(view_x*2))*(float)screen_width));
   mouse_y_rel = (int)(((float)mouse_y_rel/(float)(window_height-(view_y*2))*(float)screen_height));

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
   layer_dynamic = resizable;

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

   if(resizable)
      sdl_window = SDL_CreateWindow(title,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,width*pixel_scale,height*pixel_scale,SDL_WINDOW_RESIZABLE);
   else
      sdl_window = SDL_CreateWindow(title,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,width*pixel_scale,height*pixel_scale,0);

   if(!sdl_window)
   {
      printf("FATAL ERROR: failed to create window\n");
      exit(-1);
   }

   surface_window = SDL_GetWindowSurface(sdl_window);
   backend_update_viewport();
   SDL_SetClipRect(surface_window,&surface_base_rect);
   layer_surfaces = backend_malloc(sizeof(*layer_surfaces)*layer_num);
   memset(layer_surfaces,0,sizeof(*layer_surfaces)*layer_num);
}

//Clears the window and redraws the scene.
//Drawing is performed from back to front, layer 0 is always drawn last.
void backend_render_update()
{
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

            if(layer_surfaces[l]->w!=layers[l].type_0.target->width||layer_surfaces[l]->h!=layers[l].type_0.target->height)
            {
               SDL_FreeSurface(layer_surfaces[l]);
               layer_surfaces[l] = SDL_CreateRGBSurface(0,layers[l].type_0.render->width,layers[l].type_0.render->height,32,((SLK_Color){.r=255}).n,((SLK_Color){.g=255}).n,((SLK_Color){.b=255}).n,((SLK_Color){.a=255}.n));
            }
            SDL_LockSurface(layer_surfaces[l]);   
            memcpy(layer_surfaces[l]->pixels,layers[l].type_0.render->data,sizeof(*layers[l].type_0.render->data)*layers[l].type_0.render->width*layers[l].type_0.render->height);
            SDL_UnlockSurface(layer_surfaces[l]);   

            SDL_SetSurfaceColorMod(layer_surfaces[l],layers[l].tint.r,layers[l].tint.g,layers[l].tint.b);
            SDL_SetSurfaceAlphaMod(layer_surfaces[l],layers[l].tint.a);
            SDL_BlitScaled(layer_surfaces[l],NULL,surface_window,&dst_rect);
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
               if(layer_surfaces[l]->w!=layers[l].type_1.target->width||layer_surfaces[l]->h!=layers[l].type_1.target->height)
               {
                  SDL_FreeSurface(layer_surfaces[l]);
                  layer_surfaces[l] = SDL_CreateRGBSurface(0,layers[l].type_1.target->width,layers[l].type_1.target->height,32,((SLK_Color){.r=255}).n,((SLK_Color){.g=255}).n,((SLK_Color){.b=255}).n,((SLK_Color){.a=255}.n));
               }
               SDL_LockSurface(layer_surfaces[l]);   
               memcpy(layer_surfaces[l]->pixels,layers[l].type_1.target->data,sizeof(*layers[l].type_1.target->data)*layers[l].type_1.target->width*layers[l].type_1.target->height);
               SDL_UnlockSurface(layer_surfaces[l]);   
               layers[l].type_1.target->changed = 0;
            }
            SDL_SetSurfaceColorMod(layer_surfaces[l],layers[l].tint.r,layers[l].tint.g,layers[l].tint.b);
            SDL_SetSurfaceAlphaMod(layer_surfaces[l],layers[l].tint.a);
            SDL_BlitScaled(layer_surfaces[l],NULL,surface_window,&dst_rect);
#endif

            break;
         }
         }
      }
   }
   
   SDL_UpdateWindowSurface(sdl_window);
}

void backend_create_layer(unsigned index, int type)
{
   if(index>=layer_count)
      return;

   switch(type)
   {
   case SLK_LAYER_PAL:
#if SLK_ENABLE_PAL
      layer_surfaces[index] = SDL_CreateRGBSurface(0,screen_width,screen_height,32,((SLK_Color){.r=255}).n,((SLK_Color){.g=255}).n,((SLK_Color){.b=255}).n,((SLK_Color){.a=255}.n));
#endif
      break;
   case SLK_LAYER_RGB:
#if SLK_ENABLE_RGB
      layer_surfaces[index] = SDL_CreateRGBSurface(0,screen_width,screen_height,32,((SLK_Color){.r=255}).n,((SLK_Color){.g=255}).n,((SLK_Color){.b=255}).n,((SLK_Color){.a=255}.n));
#endif
      break;
   }
}
//-------------------------------------

#undef MAX_CONTROLLERS 
