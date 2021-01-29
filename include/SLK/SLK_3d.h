/* 
LICENSE OPTION A: 3-clause BSD

   Copyright (C) 2021 Captain4LK (Lukas Holzbeierlein)

   Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

   3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

LICENSE OPTION B: Public Domain - Unlicense

   This is free and unencumbered software released into the public domain.

   Anyone is free to copy, modify, publish, use, compile, sell, or
   distribute this software, either in source code form or as a compiled
   binary, for any purpose, commercial or non-commercial, and by any
   means.

   In jurisdictions that recognize copyright laws, the author or authors
   of this software dedicate any and all copyright interest in the
   software to the public domain. We make this dedication for the benefit
   of the public at large and to the detriment of our heirs and
   successors. We intend this dedication to be an overt act of
   relinquishment in perpetuity of all present and future rights to this
   software under copyright law.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
   OTHER DEALINGS IN THE SOFTWARE.

   For more information, please refer to <http://unlicense.org/>
*/

#ifndef _SLK_3D_H_

#define _SLK_3D_H_

#include "../../external/UtilityLK/include/ULK_vector.h"
#include "../../external/UtilityLK/include/ULK_matrix.h"
#include "../../external/UtilityLK/include/ULK_3d.h"
#include "SLK.h"

//Generic polygon
typedef struct SLK_3d_polygon
{
   ULK_vertex *vertices;
   SLK_RGB_sprite *texture_rgb;
   SLK_Pal_sprite *texture_pal;
   struct SLK_3d_polygon *next;
}SLK_3d_polygon;

//Polygon used in meshes
typedef struct SLK_3d_mesh_polygon
{
   ULK_vertex *vertices;
   int index_material_rgb;
   struct SLK_3d_mesh_polygon *next;
}SLK_3d_mesh_polygon;

typedef struct
{
   SLK_RGB_sprite **texture_rgb;
   SLK_3d_mesh_polygon *polygons;
}SLK_3d_mesh;

void SLK_3d_set_rgb_sprite_loader(SLK_RGB_sprite *(*loader)(const char *path));
SLK_3d_mesh *SLK_3d_load_obj(char *path);
void SLK_3d_draw_mesh(SLK_3d_mesh *mesh);

void SLK_3d_camera_set_pos(const ULK_vector_3d pos);
void SLK_3d_camera_set_front(const ULK_vector_3d front);
void SLK_3d_camera_set_up(const ULK_vector_3d up);
void SLK_3d_camera_update();

ULK_matrix_4x4 *SLK_3d_get_model();
ULK_matrix_4x4 *SLK_3d_get_view();
ULK_matrix_4x4 *SLK_3d_get_projection();

void SLK_3d_perspective(float fov,int width, int height, float near, float far);
void SLK_3d_translate(ULK_vector_3d translation);
void SLK_3d_rotate(ULK_vector_3d axis, float rad);
void SLK_3d_scale(float scale);
void SLK_3d_scale_3d(ULK_vector_3d scale);

void SLK_3d_init();
void SLK_3d_set_texture_rgb(SLK_RGB_sprite *tex);
void SLK_3d_set_texture_pal(SLK_Pal_sprite *tex);
void SLK_3d_start_rgb(SLK_RGB_sprite *target);
void SLK_3d_start_pal(SLK_Pal_sprite *target);

void SLK_3d_draw_poly_rgb_subaffine(ULK_vertex *verts);
void SLK_3d_draw_poly_pal_subaffine(ULK_vertex *verts);

void SLK_3d_dispatch();

#endif
