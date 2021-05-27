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
//-------------------------------------

//Internal includes
#include "../include/SLK/SLK_config.h"
#include "../include/SLK/SLK_types.h"
#include "../include/SLK/SLK_functions.h"
#include "backend.h"
#include "SLK_layer_i.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
static int running;
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
void SLK_core_set_title(const char *title)
{
   backend_set_title(title);
}

//Sets the window in windowed or fullscreen mode.
void SLK_core_set_fullscreen(int fullscreen)
{
   backend_set_fullscreen(fullscreen);
}

//Shows or hides the window.
void SLK_core_set_visible(int visible)
{
   backend_set_visible(visible);
}

//Sets the icon of the window.
void SLK_core_set_icon(const SLK_RGB_sprite *icon)
{
   backend_set_icon(icon);
}

//Returns the current viewport width.
int SLK_core_get_width()
{
   return backend_get_width();
}

//Returns the current viewport height.
int SLK_core_get_height()
{
   return backend_get_height();
}

//Updates the engine state.
//Updates the input and the timer.
void SLK_update()
{
   SLK_timer_update();

   backend_handle_events();
}

//The first function you should call in your code.
//Creates a window, sets its title and allocates space for the layers.
void SLK_setup(int width, int height, int layer_num, const char *title, int fullscreen, int scale, int resizable)
{
   //Allocate space for layers, max layer num is fixed.
   layers = malloc(sizeof(layers[0])*layer_num);
   memset(layers,0,sizeof(layers[0])*layer_num);

   backend_setup(width,height,layer_num,title,fullscreen,scale,resizable);

   running = 1;
   SLK_core_set_fullscreen(fullscreen);

   //Setup keymaps, or whatever your favourite backend does here.
   backend_input_init();
}

//Clears the window and redraws the scene.
//Drawing is performed from back to front, layer 0 is always drawn last.
void SLK_render_update()
{
   backend_render_update();
}
//-------------------------------------
