/* 
	Copyright (C) 2020 by Captain4LK (Lukas Holzbeierlein)

	This program is free software: you can redistribute it and/or modify
   	it under the terms of the GNU General Public License as published by
   	the Free Software Foundation, either version 3 of the License, or
   	(at your option) any later version.

   	This program is distributed in the hope that it will be useful,
   	but WITHOUT ANY WARRANTY; without even the implied warranty of
   	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   	GNU General Public License for more details.

   	You should have received a copy of the GNU General Public License
   	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "../../include/SLK/SLK.h"
#include "ULK_file.h"
#include "ULK_vector.h"
#include <cjson/cJSON.h>

#define WIDTH 1920
#define HEIGHT 1080

typedef struct
{
   int x;
   int y;
   int radius;
   float charge;
   char charge_str[16];
   int test_points;
}Circle;

typedef struct
{
   int type;

   union
   {
      Circle circle;
   };
}Shape;
Shape *shapes;
int shapes_count;

void load_shapes();
void draw_shapes();
void calculate_circle(int shape);
void calculate();
void calculate_pos(ULK_vector_2d out, float x, float y);

int main(int argc, char *argv[])
{
   SLK_setup(WIDTH,HEIGHT,3,"SLK Engine",1,0);
   SLK_timer_set_fps(30);
   SLK_mouse_show_cursor(1);

   SLK_layer_create(0,SLK_LAYER_RGB); //Layer for drawing shapes
   SLK_layer_create(1,SLK_LAYER_RGB); //Layer for drawing electric field
   SLK_layer_create(2,SLK_LAYER_RGB);
   SLK_layer_activate(0,1);
   SLK_layer_activate(1,0);
   SLK_layer_activate(2,1);

   load_shapes();
   draw_shapes();

   calculate();

   while(SLK_core_running())
   {
      SLK_update();

      SLK_render_update();
   }

   return 0;
}

void load_shapes()
{
   char *buffer = ULK_file_load("objects.json");
   cJSON *json = NULL;
   cJSON *circles = NULL;
   int objects_count = 0;
   int circles_count = 0;
   int i;

   if(buffer==NULL)
   {
      printf("Failed to load objects.json!\n");
      return;
   }

   json = cJSON_Parse(buffer);
   if(json==NULL)
      printf("Json file seems to be faulty!\n");

   //Load all shapes here and count them
   circles = cJSON_GetObjectItem(json,"circles");
   if(circles==NULL)
      printf("No circles specified!\n");
   circles_count = cJSON_GetArraySize(circles);
   printf("Found %d circles\n",circles_count);
   objects_count+=circles_count;

   //Allocate enough space for all shapes
   shapes_count = objects_count;
   shapes = malloc(sizeof(Shape)*shapes_count);
   objects_count = 0;
   
   //Load all circles
   for(i = 0;i<circles_count;i++)
   {
      cJSON *circle = cJSON_GetArrayItem(circles,i);

      shapes[objects_count].circle.x = cJSON_GetObjectItem(circle,"x")->valueint;
      shapes[objects_count].circle.y = cJSON_GetObjectItem(circle,"y")->valueint;
      shapes[objects_count].circle.radius = cJSON_GetObjectItem(circle,"radius")->valueint;
      shapes[objects_count].circle.charge = cJSON_GetObjectItem(circle,"charge")->valuedouble;
      shapes[objects_count].circle.test_points = cJSON_GetObjectItem(circle,"test_points")->valueint;
      shapes[objects_count].type = 0;
      sprintf(shapes[objects_count].circle.charge_str,"%07fC",shapes[objects_count].circle.charge);

      objects_count++;
   }
}

void draw_shapes()
{
   int i;

   SLK_layer_set_current(0);
   
   SLK_draw_rgb_set_clear_color(SLK_color_create(0,0,0,0));
   SLK_draw_rgb_clear();

   for(i = 0;i<shapes_count;i++)
   {
      if(shapes[i].type==0) //Circle
      {
         SLK_draw_rgb_fill_circle(shapes[i].circle.x,shapes[i].circle.y,shapes[i].circle.radius,SLK_color_create(255,255,255,255));
         SLK_draw_rgb_string(shapes[i].circle.x-72,shapes[i].circle.y-8,2,shapes[i].circle.charge_str,SLK_color_create(255,0,0,255));
      }
   }
}

void calculate()
{
   int i;

   SLK_layer_set_current(1);
   SLK_draw_rgb_set_clear_color(SLK_color_create(0,0,0,0));
   SLK_draw_rgb_clear();

   for(i = 0;i<shapes_count;i++)
   {
      switch(shapes[i].type)
      {
      case 0:
         calculate_circle(i);
         break;
      }
   }

   SLK_layer_set_current(2);
   SLK_draw_rgb_clear();

   //Calculate electrical
   for(int x = 0;x<WIDTH;x++)
   {
      for(int y = 0;y<HEIGHT;y++)
      {
         ULK_vector_2d point;
         ULK_vector_2d center;
         ULK_vector_2d_set(point,(float)x,(float)y);
         float potential = 0.0f;

         for(int i = 0;i<shapes_count;i++)
         {
            if(shapes[i].type==0)
            {
               ULK_vector_2d distance;
               float length;
              
               ULK_vector_2d_set(center,(float)shapes[i].circle.x,(float)shapes[i].circle.y);
               ULK_vector_2d_subtract(distance,center,point);
               length = ULK_vector_2d_length(distance);
               
               if(length!=0.0f)
                  potential+=8987551788.0f*(shapes[i].circle.charge/length);
            }
         }

         uint8_t color = (uint8_t)(((potential/10000000.000000f)*255.0f));
         if(color<5)
            SLK_draw_rgb_color(x,y,SLK_color_create(255,255,255,255));
      }
   }
}

void calculate_circle(int shape)
{
   int i;
   int o;

   for(i = 0;i<shapes[shape].circle.test_points;i++)
   {
      ULK_vector_2d origin;
      ULK_vector_2d point;
      ULK_vector_2d pos;
      int tries = 0;
      int negative = shapes[shape].circle.charge<0.0f;
      float angle = ((float)i/(float)shapes[shape].circle.test_points)*6.2831853f;

      ULK_vector_2d_set(origin,shapes[shape].circle.x,shapes[shape].circle.y);
      ULK_vector_2d_set(point,shapes[shape].circle.x,shapes[shape].circle.y-shapes[shape].circle.radius);
      ULK_vector_2d_rotate(point,point,origin,angle);
      ULK_vector_2d_copy(pos,point);

      while(tries<10000)
      {
         tries++;
         ULK_vector_2d norm;
         ULK_vector_2d distance;
         ULK_vector_2d result;
         ULK_vector_2d force_result;
         float force;
         float length;
         
         ULK_vector_2d_zero(force_result);

         for(o = 0;o<shapes_count;o++)
         {
            if(shapes[o].type==0)
            {
               ULK_vector_2d center;

               ULK_vector_2d_set(center,shapes[o].circle.x,shapes[o].circle.y);

               ULK_vector_2d_subtract(distance,center,pos);
               length = ULK_vector_2d_length(distance);
               ULK_vector_2d_normalize(norm,distance);
               ULK_vector_2d_scale(norm,norm,-1.0f);

               force = 8987551788.0f*(fabs(shapes[o].circle.charge)/(length*length))*0.000000001602;
               if(((shapes[o].circle.charge<0.0f)^negative)&&shape!=o)
                  ULK_vector_2d_scale(norm,norm,-force);
               else
                  ULK_vector_2d_scale(norm,norm,force);

               ULK_vector_2d_copy(result,norm);

            }

            ULK_vector_2d_add(force_result,force_result,result);
         }

         //ULK_vector_2d_scale(force_result,force_result,FACTOR);
         ULK_vector_2d_normalize(force_result,force_result);
         ULK_vector_2d_add(pos,pos,force_result);
         SLK_draw_rgb_color((int)pos[0],(int)pos[1],SLK_color_create(255,128,0,255));
      }

      printf("%d of %d calculated, %f\n",i,shapes[shape].circle.test_points,angle);
   }
}

void calculate_pos(ULK_vector_2d out, float x, float y)
{
   
}
