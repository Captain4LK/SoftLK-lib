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
#include "SLK_variables.h"

/*
 * Loads a paltte from a .pal 
 * file.
 * Support for other types
 * is planned
 */
SLK_Palette *SLK_palette_load(const char *path)
{
   SLK_Palette *palette = malloc(sizeof(SLK_Palette));

   FILE *f = fopen(path,"r");
   if(!f)
   {
      printf("Unable to load palette\n");
      return palette;
   }
   
   char buffer[512];
   int colors = 0;
   
   for(int i = 0;i<259&&fgets(buffer,512,f);i++)
   {
      if(i==2)
      {
         int found;
         sscanf(buffer,"%d",&found);
         printf("Found %d colors\n",found);
      }
      else if(i>2)
      {
         int r,g,b,a;
         sscanf(buffer,"%d %d %d %d",&r,&g,&b,&a);
         palette->colors[colors].r = r;
         palette->colors[colors].g = g;
         palette->colors[colors].b = b;
         palette->colors[colors].a = a;
         colors++;
      }
   }

   return palette;
}
