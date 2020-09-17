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
//-------------------------------------

//Internal includes
#include "../../include/SLK/SLK_types.h"
#include "../../include/SLK/SLK_functions.h"
#include "../SLK_layer_i.h"
#include "../backend.h"
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
   uint8_t new_button_state[16];
   uint8_t old_button_state[16];
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
static uint8_t gamepad_map[16];
static uint8_t new_key_state[256];
static uint8_t old_key_state[256];
static uint8_t new_mouse_state[6];
static uint8_t old_mouse_state[6];
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

//Set the window title.
void backend_set_title(const char *title)
{
   SDL_SetWindowTitle(sdl_window,title);
}

//Toggles fullscreen.
void backend_set_fullscreen(int fullscreen)
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

   backend_update_viewport();
}

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

//Sets wether the window is visible.
void backend_set_visible(int visible)
{
   if(visible)
      SDL_ShowWindow(sdl_window);
   else
      SDL_HideWindow(sdl_window);
}

//Sets the window icon.
void backend_set_icon(const SLK_RGB_sprite *icon)
{
   SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(icon->data,icon->width,icon->height,32,icon->width*4,0xf000,0x0f00,0x00f0,0x000f);

   SDL_SetWindowIcon(sdl_window,surface);

   SDL_FreeSurface(surface);
}

//Returns the viewport width adjusted to pixel scale.
int backend_get_width()
{
   return screen_width;
}

//Returns the viewport height adjusted to pixel scale.
int backend_get_height()
{
   return screen_height;
}

//Returns the view width.
int backend_get_view_width()
{
   return view_width;
}

//Returns the view height.
int backend_get_view_height()
{
   return view_height;
}

//Returns the view x pos.
int backend_get_view_x()
{
   return view_x;
}

//Returns the view y pos.
int backend_get_view_y()
{
   return view_y;
}

//Returns the window width.
int backend_get_win_width()
{
   return window_width;
}

//Returns the window height.
int backend_get_win_height()
{
   return window_height;
}

//Sets the target/maximum fps.
void backend_set_fps(int FPS)
{
   if(FPS<1||FPS>1000)
      fps = 1000;
   else
      fps = FPS;

   framedelay = 1000/fps;
}

//Returns the current target fps.
int backend_get_fps()
{
   return fps;
}

//Limits the fps to the target fps.
void backend_timer_update()
{
   frametime = SDL_GetTicks()-framestart;

   if(framedelay>frametime)
      SDL_Delay(framedelay-frametime);

   delta = (float)(SDL_GetTicks()-framestart)/1000.0f;
   framestart = SDL_GetTicks();
}

//Returns the delta time of the last frame.
float backend_timer_get_delta()
{
   return delta;
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

   layer_textures = malloc(sizeof(GLuint)*layer_num);
   memset(layer_textures,0,sizeof(GLuint)*layer_num);
}

//Creates the keymap.
void backend_input_init()
{
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
   key_map[SDL_SCANCODE_END] = SLK_KEY_END;
   key_map[SDL_SCANCODE_PAGEUP] = SLK_KEY_PGUP;
   key_map[SDL_SCANCODE_PAGEDOWN] = SLK_KEY_PGDN;
   key_map[SDL_SCANCODE_INSERT] = SLK_KEY_INS;
   key_map[SDL_SCANCODE_LSHIFT] = SLK_KEY_SHIFT;
   key_map[SDL_SCANCODE_RSHIFT] = SLK_KEY_SHIFT;
   key_map[SDL_SCANCODE_LCTRL] = SLK_KEY_CTRL;
   key_map[SDL_SCANCODE_RCTRL] = SLK_KEY_CTRL;
   key_map[SDL_SCANCODE_SPACE] = SLK_KEY_SPACE;

   key_map[SDL_SCANCODE_0] = SLK_KEY_0;
   key_map[SDL_SCANCODE_1] = SLK_KEY_1;
   key_map[SDL_SCANCODE_2] = SLK_KEY_2;
   key_map[SDL_SCANCODE_3] = SLK_KEY_3;
   key_map[SDL_SCANCODE_4] = SLK_KEY_4;
   key_map[SDL_SCANCODE_5] = SLK_KEY_5;
   key_map[SDL_SCANCODE_6] = SLK_KEY_6;
   key_map[SDL_SCANCODE_7] = SLK_KEY_7;
   key_map[SDL_SCANCODE_8] = SLK_KEY_8;
   key_map[SDL_SCANCODE_9] = SLK_KEY_9;

   key_map[SDL_SCANCODE_KP_0] = SLK_KEY_NP0;
   key_map[SDL_SCANCODE_KP_1] = SLK_KEY_NP1;
   key_map[SDL_SCANCODE_KP_2] = SLK_KEY_NP2;
   key_map[SDL_SCANCODE_KP_3] = SLK_KEY_NP3;
   key_map[SDL_SCANCODE_KP_4] = SLK_KEY_NP4;
   key_map[SDL_SCANCODE_KP_5] = SLK_KEY_NP5;
   key_map[SDL_SCANCODE_KP_6] = SLK_KEY_NP6;
   key_map[SDL_SCANCODE_KP_7] = SLK_KEY_NP7;
   key_map[SDL_SCANCODE_KP_8] = SLK_KEY_NP8;
   key_map[SDL_SCANCODE_KP_9] = SLK_KEY_NP9;
   key_map[SDL_SCANCODE_KP_MULTIPLY] = SLK_KEY_NP_MUL;
   key_map[SDL_SCANCODE_KP_PLUS] = SLK_KEY_NP_ADD;
   key_map[SDL_SCANCODE_KP_DIVIDE] = SLK_KEY_NP_DIV;
   key_map[SDL_SCANCODE_KP_MINUS] = SLK_KEY_NP_SUB;
   key_map[SDL_SCANCODE_KP_PERIOD] = SLK_KEY_NP_DECIMAL;

   mouse_map[SDL_BUTTON_LEFT] = SLK_BUTTON_LEFT;
   mouse_map[SDL_BUTTON_RIGHT] = SLK_BUTTON_RIGHT;
   mouse_map[SDL_BUTTON_MIDDLE] = SLK_BUTTON_MIDDLE;
   mouse_map[SDL_BUTTON_X1] = SLK_BUTTON_X1;
   mouse_map[SDL_BUTTON_X2] = SLK_BUTTON_X2;

   gamepad_map[SDL_CONTROLLER_BUTTON_A] = SLK_PAD_A;
   gamepad_map[SDL_CONTROLLER_BUTTON_B] = SLK_PAD_B;
   gamepad_map[SDL_CONTROLLER_BUTTON_X] = SLK_PAD_X;
   gamepad_map[SDL_CONTROLLER_BUTTON_Y] = SLK_PAD_Y;
   gamepad_map[SDL_CONTROLLER_BUTTON_BACK] = SLK_PAD_BACK;
   gamepad_map[SDL_CONTROLLER_BUTTON_GUIDE] = SLK_PAD_GUIDE;
   gamepad_map[SDL_CONTROLLER_BUTTON_START] = SLK_PAD_START;
   gamepad_map[SDL_CONTROLLER_BUTTON_LEFTSTICK] = SLK_PAD_LEFTSTICK;
   gamepad_map[SDL_CONTROLLER_BUTTON_RIGHTSTICK] = SLK_PAD_RIGHTSTICK;
   gamepad_map[SDL_CONTROLLER_BUTTON_LEFTSHOULDER] = SLK_PAD_LEFTSHOULDER;
   gamepad_map[SDL_CONTROLLER_BUTTON_RIGHTSHOULDER] = SLK_PAD_RIGHTSHOULDER;
   gamepad_map[SDL_CONTROLLER_BUTTON_DPAD_UP] = SLK_PAD_UP;
   gamepad_map[SDL_CONTROLLER_BUTTON_DPAD_DOWN] = SLK_PAD_DOWN;
   gamepad_map[SDL_CONTROLLER_BUTTON_DPAD_LEFT] = SLK_PAD_LEFT;
   gamepad_map[SDL_CONTROLLER_BUTTON_DPAD_RIGHT] = SLK_PAD_RIGHT;
   gamepad_map[SDL_CONTROLLER_BUTTON_MAX] = SLK_PAD_MAX;

   //Clear key states, just in case,
   //should already be empty since known at compile time
   memset(new_key_state,0,sizeof(new_key_state));
   memset(old_key_state,0,sizeof(old_key_state));
   memset(new_mouse_state,0,sizeof(new_mouse_state));
   memset(old_mouse_state,0,sizeof(old_mouse_state));
   for(int i = 0;i<MAX_CONTROLLERS;i++)
   {
      memset(gamepads[i].new_button_state,0,sizeof(gamepads[i].new_button_state));
      memset(gamepads[i].old_button_state,0,sizeof(gamepads[i].old_button_state));
   }
}

//Shows or hides the mouse cursor.
void backend_show_cursor(int shown)
{
   SDL_ShowCursor(shown?SDL_ENABLE:SDL_DISABLE);
}

//Sets wether the mouse cursor is captured and only relative
//mouse motion is registerd.
void backend_mouse_set_relative(int relative)
{
   SDL_SetRelativeMouseMode(relative);
}

//Sets wether to track mouse events globally.
void backend_mouse_capture(int capture)
{
   SDL_CaptureMouse(capture);
}

//Starts text input.
void backend_start_text_input(char *text)
{
   text_input = text;
   text_input_active = 1;

   SDL_StartTextInput();
}

//Stops text input.
void backend_stop_text_input()
{
   text_input_active = 0;
   
   SDL_StopTextInput();
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

int backend_key_down(int key)
{
   return new_key_state[key];
}

int backend_key_pressed(int key)
{
   return new_key_state[key]&&!old_key_state[key];
}

int backend_key_released(int key)
{
   return !new_key_state[key]&&old_key_state[key];
}

int backend_mouse_down(int key)
{
   return new_mouse_state[key];
}

int backend_mouse_pressed(int key)
{
   return new_mouse_state[key]&&!old_mouse_state[key];
}

int backend_mouse_released(int key)
{
   return !new_mouse_state[key]&&old_mouse_state[key];
}

int backend_mouse_wheel_get_scroll()
{
   return mouse_wheel;
}

int backend_gamepad_down(int index, int key)
{
   return gamepads[index].new_button_state[key];
}

int backend_gamepad_pressed(int index, int key)
{
   return gamepads[index].new_button_state[key]&&!gamepads[index].old_button_state[key];
}

int backend_gamepad_released(int index, int key)
{
   return !gamepads[index].new_button_state[key]&&gamepads[index].old_button_state[key];
}

int backend_get_gamepad_count()
{
   return SDL_NumJoysticks();
}

void backend_mouse_get_pos(int *x, int *y)
{
   *x = mouse_x;
   *y = mouse_y;
}

void backend_mouse_get_relative_pos(int *x, int *y)
{
   *x = mouse_x_rel;
   *y = mouse_y_rel;
}

static int get_gamepad_index(int which)
{

   for(int i = 0;i<MAX_CONTROLLERS;i++)
      if(gamepads[i].connected&&gamepads[i].id==which)
         return i;

   return -1;
}
//-------------------------------------

#undef MAX_CONTROLLERS 