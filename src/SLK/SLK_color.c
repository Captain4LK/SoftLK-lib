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

SLK_Color SLK_color_create(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
   SLK_Color c;
   c.r = r;
   c.g = g;
   c.b = b;
   c.a = a;
   
   return c;
} 

SLK_Paxel SLK_color_create_paxel(uint8_t index, uint8_t mask)
{
   SLK_Paxel p;
   p.index = index;
   p.mask = mask;
   
   return p;
}
