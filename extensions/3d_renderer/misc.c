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
This is were the unused software renderers go to die, 
use them however you wish.
*/

//Draws a textured n-gon to the rgb target.
void SLK_3d_draw_poly_rgb(ULK_vertex *verts)
{
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

   while(v) 
   { 
      //Move to camera
      ULK_matrix_4x4_mul_vector_3d(v->pos_pr,model,v->pos);
      ULK_matrix_4x4_mul_vector_3d(v->pos_pr,view,v->pos_pr);
      count++;

      v = v->next;
   }

   if(count<3)
      return;

   //Clip
   vn = ULK_frustum_clip(&frustum,verts,1);
   if(vn==NULL||vn->next==NULL)
   {
      ULK_vertex_reset_temp();
      return;
   }

   v = vn;
   while(v)
   {
      float z = v->pos_pr[2];
      //Project
      ULK_matrix_4x4_mul_vector_3d(v->pos_pr,projection,v->pos_pr);

      //Adjust coordinates to screen space.
      v->pos_pr[0] = X_RES_HALF+(v->pos_pr[0]*X_RES_HALF);
      v->pos_pr[1] = Y_RES_HALF+(v->pos_pr[1]*Y_RES_HALF);
      v->pos_pr[2] = z*Z_FACTOR;

      if(v->pos_pr[1]<y_min_f)
         y_min_f = v->pos_pr[1];

      if(v->pos_pr[1]>y_max_f)
         y_max_f = v->pos_pr[1];

      v = v->next;
   }

   y_min = (int)y_min_f;
   y_max = (int)y_max_f;
   if(y_min>y_max||y_min>Y_RES-1||y_max<0)
   {
      ULK_vertex_reset_temp();
      return;
   }

   for(int i = y_min;i<y_max;i++)
   {
      x_startf[i] = X_RES;
      x_endf[i] = 0;
   }

   v = vn;
   float x1 = v->pos_pr[0];
   float y1 = v->pos_pr[1];
   int z1 = (int)((1.0f/v->pos_pr[2])*POINT_PER);
   float temp = ((float)(texture_rgb->height-1)*v->u+0.5f);
   int u1 = (int)((temp/v->pos_pr[2])*POINT_PER);
   temp = ((float)(texture_rgb->width-1)*v->v+0.5f);
   int v1 = (int)((temp/v->pos_pr[2])*POINT_PER);
   int iy1 = (int)y1;
   int l;

   while(v)
   {
      ULK_vertex *next = v->next?v->next:vn;
      float x2 = next->pos_pr[0];
      float y2 = next->pos_pr[1];
      int z2 = (int)((1.0f/next->pos_pr[2])*POINT_PER);
      temp = ((float)(texture_rgb->height-1)*next->u+0.5f);
      int u2 = (int)((temp/next->pos_pr[2])*POINT_PER);
      temp = ((float)(texture_rgb->width-1)*next->v+0.5f);
      int v2 = (int)((temp/next->pos_pr[2])*POINT_PER);
      int iy2 = (int)y2;

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

   SLK_Color *fb = &target_rgb->data[y_min*X_RES];
   int *zb = &z_buffer[y_min*X_RES];

   for(int i = y_min;i<y_max;i++)
   {
#if INTERLACING

      if((i&1)==line)
      {
         fb+=X_RES;
         zb+=X_RES;
         continue;
      }

#endif

      int ilen = (int)(x_endf[i]-x_startf[i]);
      if(ilen<=0)
      {
         fb+=X_RES;
         zb+=X_RES;
         continue;
      }
      int delta_z = (z_end[i]-z_start[i])/ilen;
      int delta_u = (u_end[i]-u_start[i])/ilen;
      int delta_v = (v_end[i]-v_start[i])/ilen;

      SLK_Color *span = fb+x_start[i];
      int *spanz = zb+x_start[i];
      int z = z_start[i];
      int ut = u_start[i];
      int vt = v_start[i];

      for(int x = x_start[i];x<x_end[i];x++)
      {
         if(z>(*spanz))
         {
            *spanz = z;
            *span = texture_rgb->data[((((ut/z))&tex_mask_y)<<power)+(((vt/z))&tex_mask_x)];
         }
         span++;
         spanz++;
         z+=delta_z;
         ut+=delta_u;
         vt+=delta_v;
      }
      fb+=X_RES;
      zb+=X_RES;
   }

   ULK_vertex_reset_temp();
}

//Draws a affine textured n-gon to the rgb target.
void SLK_3d_draw_poly_rgb_affine(ULK_vertex *verts)
{
   const int tex_mask_x = texture_rgb->width-1;
   const int tex_mask_y = texture_rgb->height-1;
   const int power = (int)log2((float)texture_rgb->width);

   ULK_vertex *v = verts;
   ULK_vertex *vn = NULL;
   int y_min = Y_RES;
   int y_max = 0;
   float y_min_f = Y_RES;
   float y_max_f = 0.0f;
   int count = 0;

   while(v) 
   { 
      //Move to camera
      ULK_matrix_4x4_mul_vector_3d(v->pos_pr,model,v->pos);
      ULK_matrix_4x4_mul_vector_3d(v->pos_pr,view,v->pos_pr);
      count++;

      v = v->next;
   }

   if(count<3)
      return;

   //Clip
   vn = ULK_frustum_clip(&frustum,verts,1);
   if(vn==NULL||vn->next==NULL)
   {
      ULK_vertex_reset_temp();
      return;
   }

   v = vn;
   while(v)
   {
      float z = v->pos_pr[2];
      //Project
      ULK_matrix_4x4_mul_vector_3d(v->pos_pr,projection,v->pos_pr);

      //Adjust coordinates to screen space.
      v->pos_pr[0] = X_RES_HALF+(v->pos_pr[0]*X_RES_HALF);
      v->pos_pr[1] = Y_RES_HALF+(v->pos_pr[1]*Y_RES_HALF);
      v->pos_pr[2] = z*Z_FACTOR;

      if(v->pos_pr[1]<y_min_f)
         y_min_f = v->pos_pr[1];

      if(v->pos_pr[1]>y_max_f)
         y_max_f = v->pos_pr[1];

      v = v->next;
   }

   y_min = (int)y_min_f;
   y_max = (int)y_max_f;
   if(y_min>y_max||y_min>Y_RES-1||y_max<0)
   {
      ULK_vertex_reset_temp();
      return;
   }

   for(int i = y_min;i<y_max;i++)
   {
      x_startf[i] = X_RES;
      x_endf[i] = 0;
   }

   v = vn;
   float x1 = v->pos_pr[0];
   float y1 = v->pos_pr[1];
   int z1 = (int)((1.0f/v->pos_pr[2])*POINT_PER);
   float temp = ((float)(texture_rgb->height-1)*v->u+0.5f);
   int u1 = (int)((temp)*POINT_PER);
   temp = ((float)(texture_rgb->width-1)*v->v+0.5f);
   int v1 = (int)((temp)*POINT_PER);
   int iy1 = (int)y1;
   int l;

   while(v)
   {
      ULK_vertex *next = v->next?v->next:vn;
      float x2 = next->pos_pr[0];
      float y2 = next->pos_pr[1];
      int z2 = (int)((1.0f/next->pos_pr[2])*POINT_PER);
      temp = ((float)(texture_rgb->height-1)*next->u+0.5f);
      int u2 = (int)((temp)*POINT_PER);
      temp = ((float)(texture_rgb->width-1)*next->v+0.5f);
      int v2 = (int)((temp)*POINT_PER);
      int iy2 = (int)y2;

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

   SLK_Color *fb = &target_rgb->data[y_min*X_RES];
   int *zb = &z_buffer[y_min*X_RES];

   for(int i = y_min;i<y_max;i++)
   {
#if INTERLACING

      if((i&1)==line)
      {
         fb+=X_RES;
         zb+=X_RES;
         continue;
      }

#endif

      int ilen = (int)(x_endf[i]-x_startf[i]);
      if(ilen<=0)
      {
         fb+=X_RES;
         zb+=X_RES;
         continue;
      }
      int delta_z = (z_end[i]-z_start[i])/ilen;
      int delta_u = (u_end[i]-u_start[i])/ilen;
      int delta_v = (v_end[i]-v_start[i])/ilen;

      SLK_Color *span = fb+x_start[i];
      int *spanz = zb+x_start[i];
      int z = z_start[i];
      int ut = u_start[i];
      int vt = v_start[i];

      for(int x = x_start[i];x<x_end[i];x++)
      {
         if(z>(*spanz))
         {
            *spanz = z;
            *span = texture_rgb->data[((((ut>>16))&tex_mask_y)<<power)+(((vt>>16))&tex_mask_x)];
         }
         span++;
         spanz++;
         z+=delta_z;
         ut+=delta_u;
         vt+=delta_v;
      }
      fb+=X_RES;
      zb+=X_RES;
   }

   ULK_vertex_reset_temp();
}
