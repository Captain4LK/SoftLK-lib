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
#include "../../include/SLK/SLK.h"
#include "../../extensions/3d_renderer/SLK_3d.h"
//-------------------------------------

//Internal includes
#include "assets.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
ULK_vertex cube_temp[6][4] = 
{
   {
      {{-1.0f,-1.0f,-1.0f},{0.0f,0.0f,0.0f},1.0f,1.0f,&cube_temp[0][1]},
      {{1.0f,-1.0f,-1.0f},{0.0f,0.0f,0.0f},1.0f,0.0f,&cube_temp[0][2]},
      {{1.0f,1.0f,-1.0f},{0.0f,0.0f,0.0f},0.0f,0.0f,&cube_temp[0][3]},
      {{-1.0f,1.0f,-1.0f},{0.0f,0.0f,0.0f},0.0f,1.0f,NULL}
   },

   {
      {{1.0f,-1.0f,-1.0f},{0.0f,0.0f,0.0f},1.0f,1.0f,&cube_temp[1][1]},
      {{1.0f,-1.0f,1.0f},{0.0f,0.0f,0.0f},1.0f,0.0f,&cube_temp[1][2]},
      {{1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},0.0f,0.0f,&cube_temp[1][3]},
      {{1.0f,1.0f,-1.0f},{0.0f,0.0f,0.0f},0.0f,1.0f,NULL}
   },
   {
      {{-1.0f,-1.0f,1.0f},{0.0f,0.0f,0.0f},1.0f,0.0f,&cube_temp[2][1]},
      {{1.0f,-1.0f,1.0f},{0.0f,0.0f,0.0f},1.0f,1.0f,&cube_temp[2][2]},
      {{1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},0.0f,1.0f,&cube_temp[2][3]},
      {{-1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},0.0f,0.0f,NULL}
   },
   {
      {{-1.0f,-1.0f,-1.0f},{0.0f,0.0f,0.0f},1.0f,0.0f,&cube_temp[3][1]},
      {{-1.0f,-1.0f,1.0f},{0.0f,0.0f,0.0f},1.0f,1.0f,&cube_temp[3][2]},
      {{-1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},0.0f,1.0f,&cube_temp[3][3]},
      {{-1.0f,1.0f,-1.0f},{0.0f,0.0f,0.0f},0.0f,0.0f,NULL}
   },
   {
      {{-1.0f,1.0f,-1.0f},{0.0f,0.0f,0.0f},1.0,1.0,&cube_temp[4][1]},
      {{1.0f,1.0f,-1.0f},{0.0f,0.0f,0.0f},1.0,0.0f,&cube_temp[4][2]},
      {{1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},0.0f,0.0f,&cube_temp[4][3]},
      {{-1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},0.0f,1.0f,NULL}
   },
   {
      {{-1.0f,-1.0f,-1.0f},{0.0f,0.0f,0.0f},1.0f,1.0f,&cube_temp[5][1]},
      {{1.0f,-1.0f,-1.0f},{0.0f,0.0f,0.0f},1.0f,0.0f,&cube_temp[5][2]},
      {{1.0f,-1.0f,1.0f},{0.0f,0.0f,0.0f},0.0f,0.0f,&cube_temp[5][3]},
      {{-1.0f,-1.0f,1.0f},{0.0f,0.0f,0.0f},0.0f,1.0f,NULL}
   }
};
//-------------------------------------

//Function prototypes
static SLK_3d_polygon *create_mesh_cube(SLK_RGB_sprite *t0, SLK_RGB_sprite *t1, SLK_RGB_sprite *t2, SLK_RGB_sprite *t3, SLK_RGB_sprite *t4, SLK_RGB_sprite *t5);
//-------------------------------------

//Function implementations

void assets_load()
{
   SLK_RGB_sprite *xor = SLK_rgb_sprite_create(256,256);
   for(int y = 0;y<256;y++)
   {
      for(int x = 0;x<256;x++)
      {
         SLK_rgb_sprite_set_pixel(xor,x,y,SLK_color_create(x^y,x^y,x^y,255));
      }
   }
   skybox = create_mesh_cube(SLK_rgb_sprite_load("assets/skybox_0.png"),SLK_rgb_sprite_load("assets/skybox_1.png"),SLK_rgb_sprite_load("assets/skybox_2.png"),
                    SLK_rgb_sprite_load("assets/skybox_3.png"),SLK_rgb_sprite_load("assets/skybox_4.png"),SLK_rgb_sprite_load("assets/skybox_5.png"));
   //skybox = create_mesh_cube(xor,xor,xor,xor,xor,xor);
}

static SLK_3d_polygon *create_mesh_cube(SLK_RGB_sprite *t0, SLK_RGB_sprite *t1, SLK_RGB_sprite *t2, SLK_RGB_sprite *t3, SLK_RGB_sprite *t4, SLK_RGB_sprite *t5)
{
   SLK_3d_polygon *poly_org = malloc(sizeof(SLK_3d_polygon));
   SLK_3d_polygon *poly = poly_org;
   poly->texture_rgb = t0;
   poly->vertices = &cube_temp[0][0];
   poly->next = malloc(sizeof(SLK_3d_polygon));
   poly = poly->next;
   poly->texture_rgb = t1;
   poly->vertices = &cube_temp[1][0];
   poly->next = malloc(sizeof(SLK_3d_polygon));
   poly = poly->next;
   poly->texture_rgb = t2;
   poly->vertices = &cube_temp[2][0];
   poly->next = malloc(sizeof(SLK_3d_polygon));
   poly = poly->next;
   poly->texture_rgb = t3;
   poly->vertices = &cube_temp[3][0];
   poly->next = malloc(sizeof(SLK_3d_polygon));
   poly = poly->next;
   poly->texture_rgb = t4;
   poly->vertices = &cube_temp[4][0];
   poly->next = malloc(sizeof(SLK_3d_polygon));
   poly = poly->next;
   poly->texture_rgb = t5;
   poly->vertices = &cube_temp[5][0];
   poly->next = NULL;

   return poly_org;
}
//-------------------------------------
