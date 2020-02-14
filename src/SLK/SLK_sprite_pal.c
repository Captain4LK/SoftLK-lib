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
 * Creates a sprite with the requested dimensions
 * and returns a pointer to its location
 */
SLK_Pal_sprite *SLK_pal_sprite_create(const int width, const int height)
{
    SLK_Pal_sprite *s = malloc(sizeof(SLK_Pal_sprite));
    s->width = width;
    s->height = height;
    s->data = malloc(width*height*sizeof(SLK_Paxel));
    memset(s->data,0,sizeof(SLK_Paxel)*width*height);
    
    return s;
}

/*
 * Destroys a previously allocated sprite.
 * The user may not now that s->data also 
 * needs to be freed
 */
void SLK_pal_sprite_destroy(SLK_Pal_sprite *s)
{
   free(s->data);
   free(s);
}

/*
 * Returns the paxel at the specified
 * location.
 * The paxel layout may change in
 * the future
 */
SLK_Paxel SLK_pal_sprite_get_paxel(const SLK_Pal_sprite *s, const int x, const int y)
{
   if(INBOUNDS(0,s->width,x)&&INBOUNDS(0,s->height,y))
      return s->data[y*s->width+x];
   else
      return (SLK_Paxel){0,0};
}

/*
 * Sets the paxel at the specified
 * position.
 * Again, the paxel layout may change
 * in the future
 */
void SLK_pal_sprite_set_paxel(SLK_Pal_sprite *s, const int x, const int y, const SLK_Paxel c)
{
   if(INBOUNDS(0,s->width,x)&&INBOUNDS(0,s->height,y))
      s->data[y*s->width+x] = c;
}

/*
 * Loads a sprite from a .slk file.
 * Current layout:
 * int width,
 * int height,
 * width*height*SLK_Paxel data
 */
SLK_Pal_sprite *SLK_pal_sprite_load(const char *path)
{
   FILE *f = fopen(path,"rb");
   if(!f)
      return SLK_pal_sprite_create(1,1);
      
   int width, height;
   fread(&width,sizeof(int),1,f);
   fread(&height,sizeof(int),1,f);   
   
   SLK_Pal_sprite *s = SLK_pal_sprite_create(width,height);
   fread(s->data,sizeof(SLK_Paxel),width*height,f);
   fclose(f);
   
   return s;
}

/*
 * Saves a sprite to a file
 * with the above specified
 * layout
 */
void SLK_pal_sprite_save(const char *path, const SLK_Pal_sprite *s)
{
   FILE *f = fopen(path,"wb");
   if(!f)
      return;
      
   fwrite(&s->width,sizeof(int),1,f);
   fwrite(&s->height,sizeof(int),1,f);
   fwrite(s->data,sizeof(SLK_Paxel),s->width*s->height,f);
   fclose(f);
}

void SLK_pal_sprite_copy_partial(SLK_Pal_sprite *dst, const SLK_Pal_sprite *src, const int x, const int y, const int ox, const int oy, const int width, const int height)
{
   for(int tx = 0; tx < width; tx++)
   {
      for(int ty = 0; ty < height; ty++)
      {
         SLK_Paxel c = SLK_pal_sprite_get_paxel(src, tx + ox, ty + oy);
         SLK_pal_sprite_set_paxel(dst,x + tx, y +ty, c);
      }
   }
}

void SLK_pal_sprite_copy(SLK_Pal_sprite *dst, const SLK_Pal_sprite *src)
{
   for(int x = 0;x<src->width;x++)
      for(int y = 0;y<src->height;y++)
         SLK_pal_sprite_set_paxel(dst,x,y,SLK_pal_sprite_get_paxel(src,x,y));
}

