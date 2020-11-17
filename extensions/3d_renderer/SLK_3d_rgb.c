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

/*
Used ressources:
   * https://gitlab.com/drummyfish/small3dlib
   * https://mikro.naprvyraz.sk/docs/Coding/2/SBUFF2.TXT

*/

//External includes
#include "../../external/UtilityLK/include/ULK_vector.h"
#include "../../external/UtilityLK/include/ULK_matrix.h"
#include "../../external/UtilityLK/include/ULK_3d.h"
#include "../../include/SLK/SLK.h"
//-------------------------------------

//Internal includes
#include "../../include/SLK/SLK_3d.h"
#include "SLK_3d_config.h"
#include "SLK_3d_var.h"
//-------------------------------------

//#defines
//Macros

#define MINF(a,b) (a>b?b:a)
#define MAXF(a,b) (a<b?b:a)

#if BRANCHLESS_MACROS 

//http://graphics.stanford.edu/~seander/bithacks.html (public domain, but credit were credit is due)
#define MIN(a,b) b^((a^b)&-(a<b))

#else

#define MIN(a,b) (a>b?b:a)

#endif
//-------------------------------------

//Typedefs

#if USE_SPAN_BUFFER

typedef struct
{
   ULK_vector_3d pos[3];
   float u[3];
   float v[3];
   float sz;
   SLK_RGB_sprite *texture;
}SLK_3d_triangle;

typedef struct SLK_3d_sbuffer_tree
{
   int x0;
   int x1;
   struct SLK_3d_sbuffer_tree *left;
   struct SLK_3d_sbuffer_tree *right;
}SLK_3d_sbuffer_tree;

#endif

//-------------------------------------

//Variables
static SLK_RGB_sprite *target_rgb;
static SLK_RGB_sprite *texture_rgb;
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

//Not needed when using span buffering
static int z_buffer[X_RES*Y_RES];

#if INTERLACING

int line;

#endif

#if USE_SPAN_BUFFER

//Triangle buffer
//Keeping track of n-gons is hard, so all
//polygons get triangualized
static SLK_3d_triangle *tri_buffer;
static int tri_buffer_used;
static int tri_buffer_space;

//span_base: keep track of allocated and unused spans --> allocate less (during most frames no) new memory each frame
static SLK_3d_sbuffer_tree *span_base = NULL;
static SLK_3d_sbuffer_tree *sbuffer[Y_RES] = {NULL};

#endif

//-------------------------------------

//Function prototypes
static inline int logb2(unsigned in);

#if USE_SPAN_BUFFER

static int comp_float(const void *e0, const void* e1);
static SLK_3d_sbuffer_tree *span_new();
static void span_free(SLK_3d_sbuffer_tree *t);
static void span_draw(const SLK_3d_sbuffer_tree *t, int y, int u, int v, int z, int du, int dv, int dz, int tx, int ty, int p);
static void span_add(SLK_3d_sbuffer_tree *cur, SLK_3d_sbuffer_tree *t, int y, int u, int v, int z, int du, int dv, int dz, int tx, int ty, int p);

#endif

//-------------------------------------

//Function implementations

//Finds the log base 2 of an integer (faster than log for useless optimization purposes)
static inline int logb2(unsigned in)
{
   int r = 0;
   while(in>>=1) 
      r++;

   return r;
}

//Sets the texture used by the rgb renderer, texture must have
//power 2 dimensions, the width and height can be different though.
void SLK_3d_set_texture_rgb(SLK_RGB_sprite *tex)
{
   texture_rgb = tex;
}

//Sets the render target for the rgb renderer and clears the zbuffer, call
//before starting to draw.
void SLK_3d_start_rgb(SLK_RGB_sprite *target)
{

#if !USE_SPAN_BUFFER

   for(int i = 0;i<X_RES*Y_RES;i++)
      z_buffer[i] = -2147483647.0f;

#endif

   ULK_matrix_4x4_identity(model);
   target_rgb = target;

#if INTERLACING

   line = !line;

#endif

#if USE_SPAN_BUFFER

   tri_buffer_used = 0;
   if(tri_buffer==NULL)
   {
      tri_buffer_space = 128;
      tri_buffer = malloc(sizeof(*tri_buffer)*tri_buffer_space);
   }

   //"Free" all currently existing spans
   for(int i = 0;i<Y_RES;i++)
   {
      SLK_3d_sbuffer_tree *t = sbuffer[i];
      if(t)
         span_free(t);
      sbuffer[i] = NULL;
   }

#endif

}

//The most optimized and the recommended one!
//Use this if you don't know better.
void SLK_3d_draw_poly_rgb_subaffine(ULK_vertex *verts)
{
   if(texture_rgb==NULL)
      return;

#if !USE_SPAN_BUFFER

   const int tex_mask_x = texture_rgb->width-1;
   const int tex_mask_y = texture_rgb->height-1;
   const int power = logb2(texture_rgb->width);
   int y_min = Y_RES;
   int y_max = 0;

#endif

   ULK_vertex *v = verts;
   ULK_vertex *vn = NULL;
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
   count = 0;
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

      count++;
      v = v->next;
   }
   //-------------------------------------
  
   //Backface culling
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
   //-------------------------------------

#if USE_SPAN_BUFFER
   
   v = vn;
   for(int i = 0;i<count-2;i++)
   {
      SLK_3d_triangle g;
      ULK_vector_3d_copy(g.pos[0],vn->pos_pr);
      ULK_vector_3d_copy(g.pos[1],v->next->pos_pr);
      ULK_vector_3d_copy(g.pos[2],v->next->next->pos_pr);
      g.u[0] = vn->u;
      g.u[1] = v->next->u;
      g.u[2] = v->next->next->u;
      g.v[0] = vn->v;
      g.v[1] = v->next->v;
      g.v[2] = v->next->next->v;
      g.sz = g.pos[0][2]+g.pos[1][2]+g.pos[2][2];
      g.texture = texture_rgb;
      memcpy(&tri_buffer[tri_buffer_used],&g,sizeof(g));

      tri_buffer_used++;
      if(tri_buffer_used==tri_buffer_space)
      {
         tri_buffer_space+=128;
         tri_buffer = realloc(tri_buffer,tri_buffer_space*sizeof(*tri_buffer));
      }

      v = v->next;
   }

   ULK_vertex_reset_temp();

#else

   //Set back to defaults for later calculations.
   y_min = (int)y_min_f;
   y_max = (int)y_max_f;
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

#endif

}

void SLK_3d_dispatch()
{

#if USE_SPAN_BUFFER

#if SPAN_ZSORT

   //Sort by depth
   //Can be disabled if the engine you may
   //write on top already draws triangles from front to back.
   qsort(tri_buffer,tri_buffer_used,sizeof(*tri_buffer),comp_float);

#endif

   //Draw triangles
   for(int b = 0;b<tri_buffer_used;b++)
   {
      SLK_3d_triangle *t = &tri_buffer[b];
      texture_rgb = tri_buffer[b].texture; 
      const int tex_mask_x = texture_rgb->width-1;
      const int tex_mask_y = texture_rgb->height-1;
      const int power = logb2(texture_rgb->width);
      float y_min_f = MINF(t->pos[0][1],MINF(t->pos[1][1],t->pos[2][1]));
      float y_max_f = MAXF(t->pos[0][1],MAXF(t->pos[1][1],t->pos[2][1]));
      int y_min = (int)y_min_f;
      int y_max = (int)y_max_f;

      //Set back to defaults for later calculations.
      for(int i = y_min;i<y_max;i++)
      {
         x_startf[i] = X_RES;
         x_endf[i] = 0;
      }
      //-------------------------------------

      //Trace edges and fill tables with necessary data for drawing the polygon
      float x1 = t->pos[0][0];
      float y1 = t->pos[0][1];
      int z1 = (int)((1.0f/t->pos[0][2])*POINT_PER);
      int iy1 = (int)y1;
      int l;
      float temp = ((float)(texture_rgb->height-1)*t->u[0]+0.5f);
      int u1 = (int)(temp/t->pos[0][2]*POINT_PER);
      temp = ((float)(texture_rgb->width-1)*t->v[0]+0.5f);
      int v1 = (int)(temp/t->pos[0][2]*POINT_PER);
      for(int i = 0;i<3;i++)
      {
         int next = (i+1)<3?(i+1):0;
         float x2 = t->pos[next][0];
         float y2 = t->pos[next][1];
         int z2 = (int)((1.0f/t->pos[next][2])*POINT_PER);
         int iy2 = (int)y2;
         temp = ((float)(texture_rgb->height-1)*t->u[next]+0.5f);
         int u2 = (int)((temp/t->pos[next][2])*POINT_PER);
         temp = ((float)(texture_rgb->width-1)*t->v[next]+0.5f);
         int v2 = (int)((temp/t->pos[next][2])*POINT_PER);

         if(y2-y1==0.0f)
         {
            x1 = x2;
            y1 = y2;
            z1 = z2;
            u1 = u2;
            v1 = v2;
            iy1 = iy2;
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
      }
      //-------------------------------------

      for(int i = y_min;i<y_max;i++)
      {
         //If interlacing is enabled, we only render every second line 
         //--> half the resolution, but looks like it is full resolution.
         //DO NOT clear the screen!
#if INTERLACING

         if((i&1)==line)
         {
            continue;
         }

#endif

         const int end_x = x_end[i];
         const int start_x = x_start[i];
         const int ilen = (int)(x_endf[i]-x_startf[i]);
         if(ilen<=0)
         {
            continue;
         }
         const int delta_z = (z_end[i]-z_start[i])/ilen;
         const int delta_u = (u_end[i]-u_start[i])/ilen;
         const int delta_v = (v_end[i]-v_start[i])/ilen;

         int z = z_start[i];
         int ut = u_start[i];
         int vt = v_start[i];

         SLK_3d_sbuffer_tree *node = span_new();
         node->x0 = start_x;
         node->x1 = end_x;

         if(!sbuffer[i])
         {
            sbuffer[i] = node;
            span_draw(node,i,ut,vt,z,delta_u,delta_v,delta_z,tex_mask_x,tex_mask_y,power);
         }
         else
         {
            span_add(sbuffer[i],node,i,ut,vt,z,delta_u,delta_v,delta_z,tex_mask_x,tex_mask_y,power);
         }
      }
   }

#endif

}

#if USE_SPAN_BUFFER

static int comp_float(const void *e0, const void* e1)
{
   if((*(const SLK_3d_triangle *)e0).sz<(*(const SLK_3d_triangle *)e1).sz)
      return -1;
   return (*(const SLK_3d_triangle*)e0).sz>(*(const SLK_3d_triangle*)e1).sz;
}

static SLK_3d_sbuffer_tree *span_new()
{
   if(!span_base)
   {
      SLK_3d_sbuffer_tree *s = malloc(sizeof(*s));
      s->left = NULL;
      s->right = NULL;
      return s;
   }
   
   SLK_3d_sbuffer_tree *t = span_base;
   span_base = t->left;
   t->right = NULL;
   t->left = NULL;
   
   return t;
}

static void span_free(SLK_3d_sbuffer_tree *t)
{
   if(t->left)
      span_free(t->left);
   if(t->right)
      span_free(t->right);

   t->left = span_base;
   span_base = t;
}

static void span_draw(const SLK_3d_sbuffer_tree *t, int y, int u, int v, int z, int du, int dv, int dz, int tx, int ty, int p)
{
#if WRITE_Z_BUFFER

   int *spanz = &z_buffer[y*X_RES+t->x0];

#endif

   SLK_Color *span = &target_rgb->data[y*X_RES+t->x0];
   for(int x = t->x0;x<t->x1;x+=SUB_SPAN)
   {
      const int len = MIN((SUB_SPAN),(t->x1-x));
      const int start_u = (u/z)*POINT_PER;
      const int start_v = (v/z)*POINT_PER;
      const int end_z = (z+dz*len);
      const int end_u = ((u+du*len)/end_z)*POINT_PER;
      const int end_v = ((v+dv*len)/end_z)*POINT_PER;
      const int delta_u = (end_u-start_u)/len;
      const int delta_v = (end_v-start_v)/len;
      int su = start_u;
      int sv = start_v;

      for(int o = len;o;o--)
      {
#if WRITE_Z_BUFFER

         *spanz = z;
         spanz++;

#endif

         *span = texture_rgb->data[((((su>>POINT_PER_POWER))&ty)<<p)+(((sv>>POINT_PER_POWER))&tx)];

         span++;
         z+=dz;
         su+=delta_u;
         sv+=delta_v;
      }

      u+=du*SUB_SPAN;
      v+=dv*SUB_SPAN;
   }
}

static void span_add(SLK_3d_sbuffer_tree *cur, SLK_3d_sbuffer_tree *t, int y, int u, int v, int z, int du, int dv, int dz, int tx, int ty, int p)
{
   //While span length greater than 0
   while(t->x0<t->x1)
   {
      //To the left of current span
      if(t->x1<cur->x0)
      {
         //Early return 0: to the left of existing span(s)
         if(!cur->left)
         {
            cur->left = t;
            span_draw(t,y,u,v,z,du,dv,dz,tx,ty,p);
            return;
         }
         //Skip to next span, if not leftmost one
         else
         {
            cur = cur->left;
         }
      }
      //To the right of current span
      else if(t->x0>cur->x1)
      {
         //Early return 1: to the right of existing span(s)
         if(!cur->right)
         {
            cur->right = t;
            span_draw(t,y,u,v,z,du,dv,dz,tx,ty,p);
            return;
         }
         //Skip to next span, if not rightmost one
         else
         {
            cur = cur->right;
         }
      }
      //Starts in existing span (must be in existing span because of second if check)
      else if(t->x0>=cur->x0)
      {
         //Eearly return 2: completely in exisiting span
         if(t->x1<=cur->x1)
         {
            span_free(t);
            return;
         }

         //Clip, removes from the left of span
         if(t->x0<=cur->x1)
         {
            int diff = cur->x1-t->x0;
            u+=diff*du;
            v+=diff*dv;
            z+=diff*dz;
            t->x0 = cur->x1;

            if(!cur->right)
            {
               span_draw(t,y,u,v,z,du,dv,dz,tx,ty,p);
               cur->x1 = t->x1;
               span_free(t);
               return;
            }
            //Skip to next span if not rightmost one
            else
            {
               cur = cur->right;
            }
         }
      }
      //Starts outside and ends outside of existing span
      else if(t->x1>cur->x1)
      {
         SLK_3d_sbuffer_tree *t0 = span_new();
         *t0 = *t;
         //Clip, removes from the left of (new) span
         if(t0->x1>cur->x1)
         {
            int diff = cur->x1-t0->x0;
            int u0 = u+diff*du;
            int v0 = v+diff*dv;
            int z0 = z+diff*dz;
            t0->x0 = cur->x1;
            if(!cur->right)
            {
               span_draw(t0,y,u0,v0,z0,du,dv,dz,tx,ty,p);
               cur->x1 = t0->x1;
               span_free(t0);
            }
            //Recursion! Start a second span
            else
            {
               span_add(cur->right,t0,y,u0,v0,z0,du,dv,dz,tx,ty,p);
            }
         }
         //Completely inside current span
         if(t->x0>=cur->x0)
         {
            span_free(t);
            return;
         }

         //Clip, remove from the right of span
         t->x1 = cur->x0;

         if(!cur->left)
         {
            span_draw(t,y,u,v,z,du,dv,dz,tx,ty,p);
            cur->x0 = t->x0;
            span_free(t);
            return;
         }
         //Skip to next span if not leftmost one
         else
         {
            cur = cur->left;
         }
      }
      //Everything else
      else
      {
         if(t->x0>=cur->x0)
         {
            span_free(t);
            return;
         }

         //Clip, remove from the right of span
         t->x1 = cur->x0;

         if(!cur->left)
         {
            span_draw(t,y,u,v,z,du,dv,dz,tx,ty,p);
            cur->x0 = t->x0;
            span_free(t);
            return;
         }
         //Skip to next span if not leftmost one
         else
         {
            cur = cur->left;
         }
      }
   }
   
   span_free(t);
}

#endif

//-------------------------------------
