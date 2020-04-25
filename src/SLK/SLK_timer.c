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

//External includes
#include <SDL2/SDL.h>
//-------------------------------------

//Internal includes
#include "../../include/SLK/SLK_functions.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
int fps;
int frametime;
int framedelay;
int framestart;
float delta;
//-------------------------------------

//Function prototypes
//-------------------------------------

//Function implementations

//Sets the target fps.
//Pass a value of 0 or lower to set maximum fps.
//Hardlimited to 1000 fps because SDL_GetTicks can't go
//smaller than milliseconds.
void SLK_timer_set_fps(const int FPS)
{
   if(FPS<1||FPS>1000)
      fps = 1000;
   else
      fps = FPS;

   framedelay = 1000/fps;
}

//Returns the currently targeted fps.
//Don't know how this could be usefull,
//but anyway, here it is.
int SLK_timer_get_fps()
{
   return fps;
}

//Updates the timings and sleeps
//the needed amount of time.
//Already gets called in SLK_update,
//only use if you know
//what you are doing
void SLK_timer_update()
{
   frametime = SDL_GetTicks()-framestart;

   if(framedelay>frametime)
      SDL_Delay(framedelay-frametime);

   delta = (float)(SDL_GetTicks()-framestart)/1000.0f;
   framestart = SDL_GetTicks();
}

//Returns the time the last frame has taken
//in seconds.
//SLK is designed to use fixed framerates,
//but if you desire to do something else
//I won't stop you.
float SLK_timer_get_delta()
{
   return delta;
}
//-------------------------------------
