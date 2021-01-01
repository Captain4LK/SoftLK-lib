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
#include "../../include/SLK/SLK.h"
#include "../../include/SLK/SLK_3d.h"
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
   SLK_setup(640,480,1,"SoftLK 3d demo",0,SLK_WINDOW_MAX,0);
   SLK_mouse_set_relative(1);
   SLK_mouse_capture(1);
   SLK_timer_set_fps(60); //DO NOT set this lower (will cause headaches)

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
   speed = 0.005f;
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
      SLK_3d_dispatch();

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
   ULK_matrix_4x4_set_scale(*SLK_3d_get_model(),0.1f);
   SLK_3d_draw_mesh(house);
}
//-------------------------------------

#undef DEG2RAD
