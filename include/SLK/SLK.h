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

#ifndef _4LK_H_

#define _4LK_H_

#define SLK_log 0

//Includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "../glad/glad.h"

//Macros
//Fast integer inbounds checking
#define INBOUNDS(LOWER,UPPER,NUMBER) \
            ((unsigned)(NUMBER-LOWER)<=(UPPER-LOWER))
            
#define SIGNUM(NUM) \
   NUM==0?0:(NUM<0?-1:1)
//Two integer swapping
#define SWAP(x,y) \
            { (x)=(x)^(y); (y)=(x)^(y); (x)=(x)^(y); }

//Internal includes
#include "SLK_types.h"
#include "SLK_functions.h"

#endif //_4LK_H_
