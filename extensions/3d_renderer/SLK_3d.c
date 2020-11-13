/*
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

//External includes
#include "../../external/UtilityLK/include/ULK_vector.h"
#include "../../external/UtilityLK/include/ULK_matrix.h"
#include "../../external/UtilityLK/include/ULK_3d.h"
#include "../../include/SLK/SLK.h"
//-------------------------------------

//Internal includes
#include "../../include/SLK/SLK_3d.h"
#include "kixor/kixor_obj_parser.h"
#include "SLK_3d_config.h"
#include "SLK_3d_var.h"
//-------------------------------------

//#defines
//Macros

#if BRANCHLESS_MACROS 

//http://graphics.stanford.edu/~seander/bithacks.html (public domain, but credit were credit is due)
#define MIN(a,b) b^((a^b)&-(a<b))

#else

#define MIN(a,b) (a>b?b:a)

#endif
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
ULK_matrix_4x4 model;
ULK_matrix_4x4 view;
ULK_matrix_4x4 projection;
ULK_frustum frustum;
static ULK_vector_3d camera_pos;
static ULK_vector_3d camera_front;
static ULK_vector_3d camera_up;

static SLK_RGB_sprite *(*rgb_loader)(const char *path);
//-------------------------------------

//Function prototypes
//-------------------------------------

//Function implementations

void SLK_3d_set_rgb_sprite_loader(SLK_RGB_sprite *(*loader)(const char *path))
{
   rgb_loader = loader;
}

//Load a mesh from a obj file
//Meshes only support rgb textures for now.
SLK_3d_mesh *SLK_3d_load_obj(char *path)
{
   Kixor_obj_scene_data data;
   if(!kixor_parse_obj_scene(&data,path))
      return NULL;

   SLK_3d_mesh *m = malloc(sizeof(SLK_3d_mesh)); 
   m->polygons = malloc(sizeof(SLK_3d_polygon));
   m->texture_rgb = malloc(sizeof(SLK_RGB_sprite *)*data.material_count);

   for(int i = 0;i<data.material_count;i++)
   {
      m->texture_rgb[i] = rgb_loader(data.material_list[i]->texture_filename);
   }

   SLK_3d_mesh_polygon *cur = m->polygons;
   cur->next = NULL;
   for(int i = 0;i<data.face_count;i++)
   {
      Kixor_obj_face *f = data.face_list[i];
      cur->index_material_rgb = f->material_index;
      cur->vertices = malloc(sizeof(ULK_vertex));
      ULK_vertex *v = cur->vertices;
      v->next = NULL;
      for(int o = 0;o<f->vertex_count;o++)
      {
         int index_vert = f->vertex_index[o];
         int index_tex = f->texture_index[o];

         if(index_vert<0)
         {
            v->pos[0] = 0.0f;
            v->pos[1] = 0.0f;
            v->pos[2] = 0.0f;
         }
         else
         {
            v->pos[0] = data.vertex_list[index_vert]->e[0];
            v->pos[1] = data.vertex_list[index_vert]->e[1];
            v->pos[2] = data.vertex_list[index_vert]->e[2];
         }
         if(index_tex<0)
         {
            v->u = 0.0f;
            v->v = 0.0f;
         }
         else
         {
            v->v = data.vertex_texture_list[index_tex]->e[0];
            v->u = data.vertex_texture_list[index_tex]->e[1];
         }

         if(o!=f->vertex_count-1)
         {
            v->next = malloc(sizeof(ULK_vertex));
            v = v->next;
            v->next = NULL;
         }
      }

      if(i!=data.face_count-1)
      {
         cur->next = malloc(sizeof(SLK_3d_polygon));
         cur = cur->next;
         cur->next = NULL;
      }
   }
   kixor_delete_obj_data(&data);

   return m;
}

void SLK_3d_draw_mesh(SLK_3d_mesh *mesh)
{
   SLK_3d_mesh_polygon *p = mesh->polygons;
   while(p)
   {
      if(p->index_material_rgb>=0)
      {
         SLK_3d_set_texture_rgb(mesh->texture_rgb[p->index_material_rgb]);
         SLK_3d_draw_poly_rgb_subaffine(p->vertices);
      }

      p = p->next;
   }
}

//Update the camera pos, softlk does not handle camera
//movement, you need to do that yourself.
void SLK_3d_camera_set_pos(const ULK_vector_3d pos)
{
   ULK_vector_3d_copy(camera_pos,pos);
}

//Update the camera front direction.
void SLK_3d_camera_set_front(const ULK_vector_3d front)
{
   ULK_vector_3d_copy(camera_front,front);
}

//Update the camera up vector.
void SLK_3d_camera_set_up(const ULK_vector_3d up)
{
   ULK_vector_3d_copy(camera_up,up);
}

//Call this function after you are done changing the 
//camera pos,front,up. 
void SLK_3d_camera_update()
{
   ULK_vector_3d center;
   ULK_vector_3d_add(center,camera_pos,camera_front);
   ULK_matrix_4x4_look_at(view,camera_pos,center,camera_up);
}

//Returns the model matrix for more complex manipulation.
ULK_matrix_4x4 *SLK_3d_get_model()
{
   return &model;
}

//Returns the view matrix for more complex manipulation.
ULK_matrix_4x4 *SLK_3d_get_view()
{
   return &view;
}

//Returns the projection matrix for more complex manipulation.
ULK_matrix_4x4 *SLK_3d_get_projection()
{
   return &projection;
}

//MUST be called after SLK_3d_init() and before drawing anything.
//Creates a frustum and a projection matrix.
void SLK_3d_perspective(float fov,int width, int height, float near, float far)
{
   ULK_matrix_4x4_identity(projection);

   float aspect = ((float)width)/((float)(height));
   ULK_matrix_4x4_perspective(projection,fov,aspect,near,far);

   for(int i = 0;i<6;i++)
      if(frustum.planes[i]==NULL)
         frustum.planes[i] = malloc(sizeof(ULK_plane));

   ULK_frustum_create(&frustum,projection);
}

//Translate the model by the provided vector.
void SLK_3d_translate(ULK_vector_3d translation)
{
   ULK_matrix_4x4_translate(model,model,translation);
}

//Rotate the model along the provided axis and by the
//provied angle.
void SLK_3d_rotate(ULK_vector_3d axis, float rad)
{
   ULK_matrix_4x4_rotate(model,model,axis,rad);
}

//Scale the model along all axis with the same
//provided scale.
void SLK_3d_scale(float scale)
{
   ULK_matrix_4x4_scale(model,model,scale);
}

//Scale the model by the provided vector.
void SLK_3d_scale_3d(ULK_vector_3d scale)
{
   ULK_matrix_4x4_scale_3d(model,model,scale);
}

//Call at the start of your application, but after SLK_setup()
void SLK_3d_init()
{
   ULK_matrix_4x4_identity(model);
   ULK_matrix_4x4_identity(view);
   ULK_matrix_4x4_identity(projection);

#if INTERLACING

   line = 0;

#endif

   ULK_matrix_4x4_ortho(projection,0.0f,(float)X_RES,0.0f,(float)Y_RES,0.1f,100.0f);
}
//-------------------------------------
