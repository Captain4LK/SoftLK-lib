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
//-------------------------------------

//#defines
//Configuration:

//This software renderer can only render to a fixed size texture, since
//it could improve performance. A non-fixed size renderer will
//also be released (or has already been).
#define X_RES 640
#define Y_RES 480
#define X_RES_HALF 320
#define Y_RES_HALF 240

//Fixed point percision, must be power of 2.
#define POINT_PER 65536   
#define POINT_PER_POWER 16

//How many affine pixels to draw between perspective corrections in subaffine 
//polygon renderer.
#define SUB_SPAN 64

//Wether to use interlacing, nearly doubles rendering speed at a small loss of
//visual quality, DO NOT clear the screen while having this enabled 
//(unless you want to rape your eyes). Only recommended for resolutions 
//higher than 100x100.
#define INTERLACING 1

//Factor to multiply the z values by, this software renderer can only
//render textured polygons with a z value of less than 1 (for performance reasons).
#define Z_FACTOR 1.0f

//Use branchless macros when possible.
#define BRANCHLESS_MACROS 1

//Use backface culling
//0 for no culling
//1 for counter clockwise culling
//2 for clockwise culling
#define BACKFACE_CULLING 1

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
static ULK_matrix_4x4 model, view, projection;
static ULK_vector_3d camera_pos;
static ULK_vector_3d camera_front;
static ULK_vector_3d camera_up;
static ULK_frustum frustum;

static int z_buffer[X_RES*Y_RES];
static int x_start[Y_RES];
static float x_startf[Y_RES];
static int x_end[Y_RES];
static float x_endf[Y_RES];
static int u_start[Y_RES];
static int u_end[Y_RES];
static int v_start[Y_RES];
static int v_end[Y_RES];
static int z_start[Y_RES];
static int z_end[Y_RES];

static SLK_RGB_sprite *target_rgb;
static SLK_Pal_sprite *target_pal;
static SLK_RGB_sprite *texture_rgb;
static SLK_Pal_sprite *texture_pal;
static SLK_RGB_sprite *(*rgb_loader)(const char *path);

#if INTERLACING

static int line;

#endif
//-------------------------------------

//Function prototypes
static inline int logb2(unsigned in);
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

//Finds the log base 2 of an integer (faster than log for useless optimization purposes)
static inline int logb2(unsigned in)
{
   int r = 0;
   while(in>>=1) 
      r++;

   return r;
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

//Sets the texture used by the rgb renderer, texture must have
//power 2 dimensions, the width and height can be different though.
void SLK_3d_set_texture_rgb(SLK_RGB_sprite *tex)
{
   texture_rgb = tex;
}

//Sets the texture used by the pal renderer, texture must have
//power 2 dimensions, the width and height can be different though.
void SLK_3d_set_texture_pal(SLK_Pal_sprite *tex)
{
   texture_pal = tex;
}

//Sets the render target for the rgb renderer and clears the zbuffer, call
//before starting to draw.
void SLK_3d_start_rgb(SLK_RGB_sprite *target)
{
   for(int i = 0;i<X_RES*Y_RES;i++)
      z_buffer[i] = -2147483647;

   ULK_matrix_4x4_identity(model);
   target_rgb = target;

#if INTERLACING

   line = !line;

#endif
}

//Sets the render target for the pal renderer and clears the zbuffer, call
//before starting to draw.
void SLK_3d_start_pal(SLK_Pal_sprite *target)
{
   for(int i = 0;i<X_RES*Y_RES;i++)
      z_buffer[i] = -2147483647;

   ULK_matrix_4x4_identity(model);
   target_pal = target;

#if INTERLACING

   line = !line;

#endif
}

//The most optimized and the recommended one!
//Use this if you don't know better.
void SLK_3d_draw_poly_rgb_subaffine(ULK_vertex *verts)
{
   if(texture_rgb==NULL)
      return;
   const int tex_mask_x = texture_rgb->width-1;
   const int tex_mask_y = texture_rgb->height-1;
   const int power = logb2(texture_rgb->width);
   ULK_vertex *v = verts;
   ULK_vertex *vn = NULL;
   int y_min = Y_RES;
   int y_max = 0;
   float y_min_f = Y_RES;
   float y_max_f = 0.0f;
   int count = 0;

   //Move to camera, count vertices and abort if less than two.
   //The counting is just a fallback in case some idiot actually tries to
   //draw a 2 point shape.
   ULK_matrix_4x4 transform;
   ULK_matrix_4x4_mul(transform,view,model);
   while(v) 
   { 
      ULK_matrix_4x4_mul_vector_3d(v->pos_pr,transform,v->pos);
      count++;

      v = v->next;
   }
   if(count<3)
   {
      ULK_vertex_reset_temp();
      return;
   }
   //-------------------------------------

   //Clip against view frustum and check if less than 2 left (-->abort)
   vn = ULK_frustum_clip(&frustum,verts,1);
   if(vn==NULL||vn->next==NULL)
   {
      ULK_vertex_reset_temp();
      return;
   }
   //-------------------------------------

   //Project to screen, convert coordinates to screen space and find max/min y values.
   v = vn;
   while(v)
   {
      //Save z value, since projected one is useless.
      float z = v->pos_pr[2];

      //Project to screen.
      ULK_matrix_4x4_mul_vector_3d(v->pos_pr,projection,v->pos_pr);

      //Adjust coordinates to screen space.
      v->pos_pr[0] = X_RES_HALF+(v->pos_pr[0]*X_RES_HALF);
      v->pos_pr[1] = Y_RES_HALF+(v->pos_pr[1]*Y_RES_HALF);
      v->pos_pr[2] = z*Z_FACTOR;

      //Find min and max y value.
      if(v->pos_pr[1]<y_min_f)
         y_min_f = v->pos_pr[1];
      if(v->pos_pr[1]>y_max_f)
         y_max_f = v->pos_pr[1];

      v = v->next;
   }
   y_min = (int)y_min_f;
   y_max = (int)y_max_f;
   
#if BACKFACE_CULLING == 1

   if(ULK_vertex_winding(vn,1))
   {
      ULK_vertex_reset_temp();
      return;
   }

#elif BACKFACE_CULLING == 2

   if(!ULK_vertex_winding(vn,1))
   {
      ULK_vertex_reset_temp();
      return;
   }

#endif

   //Shouldn't happen because of frustum clipping --> commented out
   /*if(y_min>y_max||y_min>Y_RES-1||y_max<0)
   {
      ULK_vertex_reset_temp();
      return;
   }*/
   //-------------------------------------


   //Set back to defaults for later calculations.
   for(int i = y_min;i<y_max;i++)
   {
      x_startf[i] = X_RES;
      x_endf[i] = 0;
   }
   //-------------------------------------

   //Trace edges and fill tables with necessary data for drawing the polygon
   v = vn;
   float x1 = v->pos_pr[0];
   float y1 = v->pos_pr[1];
   int z1 = (int)((1.0f/v->pos_pr[2])*POINT_PER);
   int iy1 = (int)y1;
   int l;
   float temp = ((float)(texture_rgb->height-1)*v->u+0.5f);
   int u1 = (int)(temp/v->pos_pr[2]*POINT_PER);
   temp = ((float)(texture_rgb->width-1)*v->v+0.5f);
   int v1 = (int)(temp/v->pos_pr[2]*POINT_PER);
   while(v)
   {
      ULK_vertex *next = v->next?v->next:vn;
      float x2 = next->pos_pr[0];
      float y2 = next->pos_pr[1];
      int z2 = (int)((1.0f/next->pos_pr[2])*POINT_PER);
      int iy2 = (int)y2;
      temp = ((float)(texture_rgb->height-1)*next->u+0.5f);
      int u2 = (int)((temp/next->pos_pr[2])*POINT_PER);
      temp = ((float)(texture_rgb->width-1)*next->v+0.5f);
      int v2 = (int)((temp/next->pos_pr[2])*POINT_PER);

      if(y2-y1==0.0f)
      {
         x1 = x2;
         y1 = y2;
         z1 = z2;
         u1 = u2;
         v1 = v2;
         iy1 = iy2;
         v = v->next;
         continue;
      }

      float ylen = 1.0f/(y2-y1);
      float delta_x = (x2-x1)*ylen;
      int delta_z = (z2-z1)*ylen;
      int delta_u = (u2-u1)*ylen;
      int delta_v = (v2-v1)*ylen;

      if(iy2>iy1)
      {
         l = iy2-iy1;
      }
      else
      {
         l = iy1-iy2;
         x1 = x2;
         z1 = z2;
         u1 = u2;
         v1 = v2;
         iy1 = iy2;
      }

      for(int s = 0;s<l;s++)
      {
         if(x1<x_startf[iy1])
         {
            x_start[iy1] = (int)x1;
            x_startf[iy1] = x1;
            z_start[iy1] = z1;
            u_start[iy1] = u1;
            v_start[iy1] = v1;
         }
         if(x1>x_endf[iy1])
         {
            x_end[iy1] = (int)x1;
            x_endf[iy1] = x1;
            z_end[iy1] = z1;
            u_end[iy1] = u1;
            v_end[iy1] = v1;
         }

         x1+=delta_x;
         z1+=delta_z;
         u1+=delta_u;
         v1+=delta_v;
         iy1++;
      }

      x1 = x2;
      y1 = y2;
      z1 = z2;
      u1 = u2;
      v1 = v2;
      iy1 = iy2;
      v = v->next;
   }
   //-------------------------------------

   SLK_Color *fb = &target_rgb->data[y_min*X_RES];
   int *zb = &z_buffer[y_min*X_RES];
   for(int i = y_min;i<y_max;i++)
   {
      //If interlacing is enabled, we only render every second line 
      //--> half the resolution, but looks like it is full resolution.
      //DO NOT clear the screen!
#if INTERLACING

      if((i&1)==line)
      {
         fb+=X_RES;
         zb+=X_RES;
         continue;
      }

#endif

      const int end_x = x_end[i];
      const int start_x = x_start[i];
      const int ilen = (int)(x_endf[i]-x_startf[i]);
      if(ilen<=0)
      {
         fb+=X_RES;
         zb+=X_RES;
         continue;
      }
      const int delta_z = (z_end[i]-z_start[i])/ilen;
      const int delta_u = (u_end[i]-u_start[i])/ilen;
      const int delta_v = (v_end[i]-v_start[i])/ilen;

      SLK_Color *span = fb+start_x;
      int *spanz = zb+start_x;
      int z = z_start[i];
      int ut = u_start[i];
      int vt = v_start[i];

      for(int x = start_x;x<end_x;x+=SUB_SPAN)
      {
         const int len = MIN((SUB_SPAN),(end_x-x));
         const int start_u = (ut/z)*POINT_PER;
         const int start_v = (vt/z)*POINT_PER;
         const int end_z = (z+delta_z*len);
         const int end_u = ((ut+delta_u*len)/end_z)*POINT_PER;
         const int end_v = ((vt+delta_v*len)/end_z)*POINT_PER;
         const int du = (end_u-start_u)/len;
         const int dv = (end_v-start_v)/len;
         int su = start_u;
         int sv = start_v;

         for(int o = len;o;o--)
         {
            if(z>(*spanz))
            {
               *spanz = z;
               *span = texture_rgb->data[((((su>>POINT_PER_POWER))&tex_mask_y)<<power)+(((sv>>POINT_PER_POWER))&tex_mask_x)];
            }

            span++;
            spanz++;
            z+=delta_z;
            su+=du;
            sv+=dv;
         }

         ut+=delta_u*SUB_SPAN;
         vt+=delta_v*SUB_SPAN;
      }
      fb+=X_RES;
      zb+=X_RES;
   }

   ULK_vertex_reset_temp();
}

void SLK_3d_draw_poly_pal_subaffine(ULK_vertex *verts)
{
   const int tex_mask_x = texture_pal->width-1;
   const int tex_mask_y = texture_pal->height-1;
   const int power = logb2(texture_pal->width);
   ULK_vertex *v = verts;
   ULK_vertex *vn = NULL;
   int y_min = Y_RES;
   int y_max = 0;
   float y_min_f = Y_RES;
   float y_max_f = 0.0f;
   int count = 0;

   //Move to camera, count vertices and abort if less than two.
   //The counting is just a fallback in case some idiot actually tries to
   //draw a 2 point shape.
   ULK_matrix_4x4 transform;
   ULK_matrix_4x4_mul(transform,view,model);
   while(v) 
   { 
      ULK_matrix_4x4_mul_vector_3d(v->pos_pr,transform,v->pos);
      count++;

      v = v->next;
   }
   if(count<3)
   {
      ULK_vertex_reset_temp();
      return;
   }
   //-------------------------------------

   //Clip against view frustum and check if less than 2 left (-->abort)
   vn = ULK_frustum_clip(&frustum,verts,1);
   if(vn==NULL||vn->next==NULL)
   {
      ULK_vertex_reset_temp();
      return;
   }
   //-------------------------------------

   //Project to screen, convert coordinates to screen space and find max/min y values.
   v = vn;
   while(v)
   {
      //Save z value, since projected one is useless.
      float z = v->pos_pr[2];

      //Project to screen.
      ULK_matrix_4x4_mul_vector_3d(v->pos_pr,projection,v->pos_pr);

      //Adjust coordinates to screen space.
      v->pos_pr[0] = X_RES_HALF+(v->pos_pr[0]*X_RES_HALF);
      v->pos_pr[1] = Y_RES_HALF+(v->pos_pr[1]*Y_RES_HALF);
      v->pos_pr[2] = z*Z_FACTOR;

      //Find min and max y value.
      if(v->pos_pr[1]<y_min_f)
         y_min_f = v->pos_pr[1];
      if(v->pos_pr[1]>y_max_f)
         y_max_f = v->pos_pr[1];

      v = v->next;
   }
   y_min = (int)y_min_f;
   y_max = (int)y_max_f;
   //Shouldn't happen because of frustum clipping --> commented out
   /*if(y_min>y_max||y_min>Y_RES-1||y_max<0)
   {
      ULK_vertex_reset_temp();
      return;
   }*/
   //-------------------------------------


   //Set back to defaults for later calculations.
   for(int i = y_min;i<y_max;i++)
   {
      x_startf[i] = X_RES;
      x_endf[i] = 0;
   }
   //-------------------------------------

   //Trace edges and fill tables with necessary data for drawing the polygon
   v = vn;
   float x1 = v->pos_pr[0];
   float y1 = v->pos_pr[1];
   int z1 = (int)((1.0f/v->pos_pr[2])*POINT_PER);
   int iy1 = (int)y1;
   int l;
   float temp = ((float)(texture_pal->height-1)*v->u+0.5f);
   int u1 = (int)(temp/v->pos_pr[2]*POINT_PER);
   temp = ((float)(texture_pal->width-1)*v->v+0.5f);
   int v1 = (int)(temp/v->pos_pr[2]*POINT_PER);
   while(v)
   {
      ULK_vertex *next = v->next?v->next:vn;
      float x2 = next->pos_pr[0];
      float y2 = next->pos_pr[1];
      int z2 = (int)((1.0f/next->pos_pr[2])*POINT_PER);
      int iy2 = (int)y2;
      temp = ((float)(texture_pal->height-1)*next->u+0.5f);
      int u2 = (int)((temp/next->pos_pr[2])*POINT_PER);
      temp = ((float)(texture_pal->width-1)*next->v+0.5f);
      int v2 = (int)((temp/next->pos_pr[2])*POINT_PER);

      if(y2-y1==0.0f)
      {
         x1 = x2;
         y1 = y2;
         z1 = z2;
         u1 = u2;
         v1 = v2;
         iy1 = iy2;
         v = v->next;
         continue;
      }

      float ylen = 1.0f/(y2-y1);
      float delta_x = (x2-x1)*ylen;
      int delta_z = (z2-z1)*ylen;
      int delta_u = (u2-u1)*ylen;
      int delta_v = (v2-v1)*ylen;

      if(iy2>iy1)
      {
         l = iy2-iy1;
      }
      else
      {
         l = iy1-iy2;
         x1 = x2;
         z1 = z2;
         u1 = u2;
         v1 = v2;
         iy1 = iy2;
      }

      for(int s = 0;s<l;s++)
      {
         if(x1<x_startf[iy1])
         {
            x_start[iy1] = (int)x1;
            x_startf[iy1] = x1;
            z_start[iy1] = z1;
            u_start[iy1] = u1;
            v_start[iy1] = v1;
         }
         if(x1>x_endf[iy1])
         {
            x_end[iy1] = (int)x1;
            x_endf[iy1] = x1;
            z_end[iy1] = z1;
            u_end[iy1] = u1;
            v_end[iy1] = v1;
         }

         x1+=delta_x;
         z1+=delta_z;
         u1+=delta_u;
         v1+=delta_v;
         iy1++;
      }

      x1 = x2;
      y1 = y2;
      z1 = z2;
      u1 = u2;
      v1 = v2;
      iy1 = iy2;
      v = v->next;
   }
   //-------------------------------------

   SLK_Paxel *fb = &target_pal->data[y_min*X_RES];
   int *zb = &z_buffer[y_min*X_RES];
   for(int i = y_min;i<y_max;i++)
   {
      //If interlacing is enabled, we only render every second line 
      //--> half the resolution, but looks like it is full resolution.
      //DO NOT clear the screen!
#if INTERLACING

      if((i&1)==line)
      {
         fb+=X_RES;
         zb+=X_RES;
         continue;
      }

#endif

      const int end_x = x_end[i];
      const int start_x = x_start[i];
      const int ilen = (int)(x_endf[i]-x_startf[i]);
      if(ilen<=0)
      {
         fb+=X_RES;
         zb+=X_RES;
         continue;
      }
      const int delta_z = (z_end[i]-z_start[i])/ilen;
      const int delta_u = (u_end[i]-u_start[i])/ilen;
      const int delta_v = (v_end[i]-v_start[i])/ilen;

      SLK_Paxel *span = fb+start_x;
      int *spanz = zb+start_x;
      int z = z_start[i];
      int ut = u_start[i];
      int vt = v_start[i];

      for(int x = start_x;x<end_x;x+=SUB_SPAN)
      {
         const int len = MIN((SUB_SPAN),(end_x-x));
         const int start_u = (ut/z)*POINT_PER;
         const int start_v = (vt/z)*POINT_PER;
         const int end_z = (z+delta_z*len);
         const int end_u = ((ut+delta_u*len)/end_z)*POINT_PER;
         const int end_v = ((vt+delta_v*len)/end_z)*POINT_PER;
         const int du = (end_u-start_u)/len;
         const int dv = (end_v-start_v)/len;
         int su = start_u;
         int sv = start_v;

         for(int o = 0;o<len;o++)
         {
            if(z>(*spanz))
            {
               *spanz = z;
               *span = texture_pal->data[((((su>>POINT_PER_POWER))&tex_mask_y)<<power)+(((sv>>POINT_PER_POWER))&tex_mask_x)];
            }

            span++;
            spanz++;
            z+=delta_z;
            su+=du;
            sv+=dv;
         }

         ut+=delta_u*SUB_SPAN;
         vt+=delta_v*SUB_SPAN;
      }
      fb+=X_RES;
      zb+=X_RES;
   }

   ULK_vertex_reset_temp();
}
//-------------------------------------

#undef X_RES
#undef Y_RES
#undef POINT_PER
#undef INTERLACING
#undef Z_FACTOR
#undef SUB_SPAN
#undef MIN
