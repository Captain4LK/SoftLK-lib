#include "../../include/SLK/SLK.h"

int main(int argc, char *argv[])
{
   SLK_setup(800,600,2,"SoftLK resize test",0,1,1);
   SLK_timer_set_fps(60);

   SLK_layer_create(1,SLK_LAYER_RGB);
   SLK_layer_activate(1,1);
   SLK_layer_set_current(1);
   SLK_layer_set_dynamic(1,1);
   SLK_draw_rgb_set_clear_color(SLK_color_create(128,128,128,255));
   SLK_draw_rgb_clear();

   int canvas_pos_x = 128;
   int canvas_pos_y = 128;
   float canvas_scale = 2.0f;
   SLK_layer_create(0,SLK_LAYER_RGB);
   SLK_layer_activate(0,1);
   SLK_layer_set_current(0);
   SLK_draw_rgb_set_changed(1);
   SLK_layer_set_dynamic(0,0);
   SLK_layer_set_size(0,256,224);
   SLK_layer_set_pos(0,canvas_pos_x,canvas_pos_y);
   SLK_layer_set_scale(0,canvas_scale);
   SLK_draw_rgb_set_clear_color(SLK_color_create(255,255,255,255));
   SLK_draw_rgb_clear();
   SLK_RGB_sprite *logo = SLK_rgb_sprite_load("assets/logo.png");
   SLK_draw_rgb_sprite(logo,0,0);
   SLK_rgb_sprite_destroy(logo);

   while(SLK_core_running())
   {
      SLK_update();

      if(SLK_mouse_down(SLK_BUTTON_MIDDLE))
      {
         int x,y;
         SLK_mouse_get_relative_pos(&x,&y);
         canvas_pos_x+=x;
         canvas_pos_y+=y;
         SLK_layer_set_pos(0,canvas_pos_x,canvas_pos_y);
      }

      if(SLK_key_down(SLK_KEY_CTRL))
      {
         int wheel = SLK_mouse_wheel_get_scroll();

         if(wheel<0)
         {
            canvas_scale+=canvas_scale*0.1f;
            SLK_layer_set_scale(0,canvas_scale);
         }
         else if(wheel>0)
         {
            canvas_scale-=canvas_scale*0.1f;
            SLK_layer_set_scale(0,canvas_scale);
         }
      }

      SLK_layer_set_current(1);
      SLK_draw_rgb_set_changed(1);
      SLK_draw_rgb_set_clear_color(SLK_color_create(128,128,128,255));
      SLK_draw_rgb_clear();

      SLK_render_update();
   }

   return 0;
}
