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
#define DEG2RAD(a) \
   ((a/180.0f)*3.1415927f)
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
static float pitch;
static float yaw;
static float speed;
static float sensitivity;
static ULK_vector_3d pos;
static ULK_vector_3d front;
static ULK_vector_3d up;
//-------------------------------------

//Function prototypes
static void camera_update();
static void draw();
//-------------------------------------

//Function implementations
int main()
{
   SLK_setup(640,480,1,"SoftLK 3d demo",1,SLK_WINDOW_MAX,0);
   SLK_mouse_set_relative(1);
   SLK_mouse_capture(1);
   SLK_timer_set_fps(60);

   SLK_layer_create(0,SLK_LAYER_RGB);
   SLK_layer_set_dynamic(0,0);
   SLK_layer_activate(0,1);
   SLK_layer_set_current(0);
   SLK_draw_rgb_set_changed(1);
   SLK_draw_rgb_set_clear_color(SLK_color_create(255,255,255,255));
   SLK_draw_rgb_clear();

   SLK_3d_init();
   SLK_3d_perspective(1.57079633,640,480,0.1f,100.0f);

   //Setup camera
   ULK_vector_3d_set(pos,0.0f,0.0f,0.0f);
   ULK_vector_3d_set(front,0.0f,0.0f,-1.0f);
   ULK_vector_3d_set(up,0.0f,1.0f,0.0f);
   speed = 0.001f;
   yaw = -90.0f;
   pitch = -58.0f;
   sensitivity = 0.5f;
   SLK_3d_camera_set_pos(pos);
   SLK_3d_camera_set_up(up);
   SLK_3d_camera_set_front(front);
   SLK_3d_camera_update();

   assets_load();

   while(SLK_core_running())
   {
      SLK_update();

      SLK_layer_set_current(0);
      SLK_draw_rgb_set_changed(1);
      SLK_3d_start_rgb(SLK_layer_get(0)->type_1.target);
      camera_update();
      draw();

      SLK_render_update();
   }

   return 0;
}

static void camera_update()
{
   ULK_vector_3d cross;
   ULK_vector_3d z_speed;
   ULK_vector_3d_cross(cross,front,up);
   ULK_vector_3d_norm(z_speed,cross);

   if(SLK_key_down(SLK_KEY_W))
   {
      pos[2]+=speed*front[2];
      pos[1]+=speed*front[1];
      pos[0]+=speed*front[0];
   }
   if(SLK_key_down(SLK_KEY_S))
   {
      pos[2]-=speed*front[2];
      pos[1]-=speed*front[1];
      pos[0]-=speed*front[0];
   }
   if(SLK_key_down(SLK_KEY_A))
   {
      pos[2]-=speed*z_speed[2];
      pos[1]-=speed*z_speed[1];
      pos[0]-=speed*z_speed[0];
   }
   if(SLK_key_down(SLK_KEY_D))
   {
      pos[2]+=speed*z_speed[2];
      pos[1]+=speed*z_speed[1];
      pos[0]+=speed*z_speed[0];
   }

   int x,y;
   SLK_mouse_get_relative_pos(&x,&y);

   float x_off = ((float)x)*sensitivity;
   float y_off = ((float)y)*sensitivity;

   yaw+=x_off;
   pitch+=y_off;
   if(pitch>89.0f)
      pitch = 89.0f;
   if(pitch<-89.0f)
      pitch = -89.0f;

   ULK_vector_3d tfront;
   tfront[0] = cosf(DEG2RAD(yaw))*cosf(DEG2RAD(pitch));
   tfront[1] = sinf(DEG2RAD(pitch));
   tfront[2] = sinf(DEG2RAD(yaw))*cosf(DEG2RAD(pitch));
   ULK_vector_3d_norm(front,tfront);

   SLK_3d_camera_set_pos(pos);
   SLK_3d_camera_set_up(up);
   SLK_3d_camera_set_front(front);

   SLK_3d_camera_update();
}

static void draw()
{
   //ULK_vector_3d trans; 
   //ULK_vector_3d_negate(trans,pos);
   //ULK_matrix_4x4_set_translation(*SLK_3d_get_model(),trans);
   ULK_matrix_4x4_scale(*SLK_3d_get_model(),*SLK_3d_get_model(),0.1f);
   SLK_3d_polygon *p = skybox;
   while(p)
   {
      SLK_3d_set_texture_rgb(p->texture_rgb);
      SLK_3d_draw_poly_rgb_subaffine(p->vertices);

      p = p->next;
   }
}
//-------------------------------------

#undef DEG2RAD
