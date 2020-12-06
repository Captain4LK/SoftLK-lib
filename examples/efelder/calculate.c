/* 
   Copyright (C) 2020 by Captain4LK (Lukas Holzbeierlein)

   This program is free software: you can redistribute it and/or modify
      it under the terms of the GNU General Public License as published by
      the Free Software Foundation, either version 3 of the License, or
      the Free Software Foundation, either version 3 of the License, or
      (at your option) any later version.

      This program is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
      GNU General Public License for more details.

      You should have received a copy of the GNU General Public License
      along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

//External includes
#include <stdio.h>
#include <stdlib.h>
#include "../../external/UtilityLK/include/ULK_vector.h"
#include "../../external/UtilityLK/include/ULK_json.h"
//-------------------------------------

//Internal includes
#include "../../include/SLK/SLK.h"
#include "settings.h"
#include "calculate.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
Shape *shapes;
int shapes_count;
Rectangle *rectangles_tmp;
int rectangles_tmp_count;
SLK_RGB_sprite *key_potential;

static int potential_mode = 0;
static float divider_potential = 1000000.0f;
static int manual_potential = 0;
static float charge_min;
static float charge_max;
//-------------------------------------

//Function prototypes
static void calculate_potential_0();
static void calculate_potential_1();
static void calculate_circle(int shape);
static void draw_shapes();
//-------------------------------------

//Function implementations

void shapes_load_file(const char *path)
{
   ULK_json5_root *json = NULL;
   ULK_json5 *circles = NULL;
   ULK_json5 *rectangles = NULL;
   int objects_count = 0;
   int circles_count = 0;
   int rectangles_count = 0;

   json = ULK_json_parse_file(path);
   if(json==NULL)
      printf("Json file seems to be faulty!\n");

   //Load all shapes here and count them
   circles = ULK_json_get_object(&json->root,"circles");
   if(circles==NULL)
      printf("No circles specified!\n");
   circles_count = ULK_json_get_array_size(circles);
   printf("Found %d circles\n",circles_count);
   objects_count+=circles_count;
   rectangles = ULK_json_get_object(&json->root,"rectangles");
   if(rectangles==NULL)
      printf("No rectangles specified!\n");
   rectangles_count = ULK_json_get_array_size(rectangles);
   printf("Found %d rectangles\n",rectangles_count);
   for(int i = 0;i<rectangles_count;i++)
   {
      ULK_json5 *rectangle = ULK_json_get_array_item(rectangles,i);
      objects_count+=ULK_json_get_object(rectangle,"divisions")->integer;
   }

   //Allocate enough space for all shapes
   shapes_count = objects_count;
   shapes = malloc(sizeof(Shape)*shapes_count);
   rectangles_tmp_count = rectangles_count;
   rectangles_tmp = malloc(sizeof(Rectangle)*rectangles_count);
   objects_count = 0;

   //Set how the potential is supposed to be drawn
   potential_mode = ULK_json_get_object(&json->root,"mode")->integer;
   divider_potential = ULK_json_get_object(&json->root,"divisions")->real;
   manual_potential = ULK_json_get_object(&json->root,"manual_potential")->integer;
   if(manual_potential)
   {
      charge_min = ULK_json_get_object(&json->root,"charge_min")->real;
      charge_max = ULK_json_get_object(&json->root,"charge_max")->real;
   }
   
   //Load all circles
   for(int i = 0;i<circles_count;i++)
   {
      ULK_json5 *circle = ULK_json_get_array_item(circles,i);

      shapes[objects_count].circle.x = ULK_json_get_object(circle,"x")->integer;
      shapes[objects_count].circle.y = ULK_json_get_object(circle,"y")->integer;
      shapes[objects_count].circle.color.r = ULK_json_get_object(circle,"r")->integer;
      shapes[objects_count].circle.color.g = ULK_json_get_object(circle,"g")->integer;
      shapes[objects_count].circle.color.b = ULK_json_get_object(circle,"b")->integer;
      shapes[objects_count].circle.color.a = 255;
      shapes[objects_count].circle.color_inv.r = 255-shapes[objects_count].circle.color.r;
      shapes[objects_count].circle.color_inv.g = 255-shapes[objects_count].circle.color.g;
      shapes[objects_count].circle.color_inv.b = 255-shapes[objects_count].circle.color.b;
      shapes[objects_count].circle.color_inv.a = 255;
      shapes[objects_count].circle.radius = ULK_json_get_object(circle,"radius")->integer;
      shapes[objects_count].circle.charge = ULK_json_get_object(circle,"charge")->real;
      shapes[objects_count].circle.test_points = ULK_json_get_object(circle,"test_points")->integer;
      shapes[objects_count].type = 0;
      sprintf(shapes[objects_count].circle.charge_str,"%07fC",shapes[objects_count].circle.charge);

      objects_count++;
   }

   //Load all rectangles
   for(int i = 0;i<rectangles_count;i++)
   {
      ULK_json5 *rectangle = ULK_json_get_array_item(rectangles,i);

      int x = ULK_json_get_object(rectangle,"x")->integer;
      int y = ULK_json_get_object(rectangle,"y")->integer;
      int width = ULK_json_get_object(rectangle,"width")->integer;
      int height = ULK_json_get_object(rectangle,"height")->integer;
      int divisions = ULK_json_get_object(rectangle,"divisions")->integer;
      float charge = (ULK_json_get_object(rectangle,"charge")->real)/(double)(divisions);
      int test_points = ULK_json_get_object(rectangle,"test_points")->integer;
      int radius = width>height?height:width;
      SLK_Color color;
      color.r = ULK_json_get_object(rectangle,"r")->integer;
      color.g = ULK_json_get_object(rectangle,"g")->integer;
      color.b = ULK_json_get_object(rectangle,"b")->integer;
      color.a = 255;
      rectangles_tmp[i].x = x;
      rectangles_tmp[i].y = y;
      rectangles_tmp[i].width = width;
      rectangles_tmp[i].height = height;
      rectangles_tmp[i].charge = charge*divisions;
      rectangles_tmp[i].color.n = color.n;
      rectangles_tmp[i].color_inv = SLK_color_create(255-color.r,255-color.g,255-color.b,255);
      sprintf(rectangles_tmp[i].charge_str,"%07fC",charge*divisions);
      float lerp_val;
      float av_points = (float)(test_points)/(float)(divisions);
      float error = 0.0f;
      if(width>height)
         lerp_val = (float)(width-radius)/(float)divisions;
      else
         lerp_val = (float)(height-radius)/(float)divisions;

      for(int o = 0;o<divisions;o++)
      {
         if(width>height)
         {
            shapes[objects_count].circle.x = (int)(lerp_val*(float)o)+x+radius/2;
            shapes[objects_count].circle.y = y+radius/2;
         }
         else
         {
            shapes[objects_count].circle.y = (int)(lerp_val*(float)o)+y+radius/2;
            shapes[objects_count].circle.x = x+radius/2;
         }
         shapes[objects_count].circle.radius = radius/2;
         shapes[objects_count].circle.charge = charge;
         shapes[objects_count].circle.color.n = color.n;
         shapes[objects_count].circle.color_inv.n = rectangles_tmp[i].color_inv.n;
         shapes[objects_count].circle.test_points = (int)(av_points+error);
         error+=(av_points-shapes[objects_count].circle.test_points);
         shapes[objects_count].type = 1;
         sprintf(shapes[objects_count].circle.charge_str,"%07fC",shapes[objects_count].circle.charge);

         objects_count++;
      }
   }

   ULK_json_free(json);
}

void calculate()
{
   SLK_core_set_visible(0);
   SLK_Layer *layer0, *layer1, *layer2;
   layer0 = SLK_layer_get(1);
   layer1 = SLK_layer_get(2);
   layer2 = SLK_layer_get(3);
   if(layer0->type_1.target->width!=canvas_width||layer0->type_1.target->height!=canvas_height)
      SLK_layer_set_size(1,canvas_width,canvas_height);
   if(layer1->type_1.target->width!=canvas_width||layer1->type_1.target->height!=canvas_height)
      SLK_layer_set_size(2,canvas_width,canvas_height);
   if(layer2->type_1.target->width!=canvas_width||layer2->type_1.target->height!=canvas_height)
      SLK_layer_set_size(3,canvas_width,canvas_height);

   SLK_draw_rgb_set_clear_color(SLK_color_create(0,0,0,0));
   SLK_layer_set_current(1);
   SLK_draw_rgb_clear();
   SLK_layer_set_current(2);
   SLK_draw_rgb_clear();
   SLK_layer_set_current(3);
   SLK_draw_rgb_clear();
   draw_shapes();

   int i;

   SLK_layer_set_current(2);
   SLK_draw_rgb_set_changed(1);
   SLK_draw_rgb_set_clear_color(SLK_color_create(0,0,0,0));
   SLK_draw_rgb_clear();

   printf("\nCalculating electrical field: \n");
   for(i = 0;i<shapes_count;i++)
      calculate_circle(i);
   puts("");

   SLK_layer_set_current(3);
   SLK_draw_rgb_set_changed(1);
   SLK_draw_rgb_clear();

   //Calculate electrical potential
   printf("Calculating electric potential: \n");
   if(potential_mode==0)
   {
      calculate_potential_0();
   }
   else
   {
      calculate_potential_1();
      calculate_potential_0();
   }

   SLK_core_set_visible(1);
}

static void calculate_circle(int shape)
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
      ULK_vector_2d_rot(point,point,origin,angle);
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
            ULK_vector_2d center;

            ULK_vector_2d_set(center,shapes[o].circle.x,shapes[o].circle.y);

            ULK_vector_2d_sub(distance,center,pos);
            length = ULK_vector_2d_mag(distance);
            ULK_vector_2d_norm(norm,distance);
            ULK_vector_2d_mul(norm,norm,-1.0f);

            force = 8987551788.0f*(fabs(shapes[o].circle.charge)/(length*length))*0.000000001602;
            if(((shapes[o].circle.charge<0.0f)^negative)&&shape!=o)
               ULK_vector_2d_mul(norm,norm,-force);
            else
               ULK_vector_2d_mul(norm,norm,force);

            ULK_vector_2d_copy(result,norm);

            ULK_vector_2d_add(force_result,force_result,result);
         }

         ULK_vector_2d_norm(force_result,force_result);
         if(tries%200==0)
         {
            ULK_vector_2d arrow_0;
            ULK_vector_2d arrow_1;
            ULK_vector_2d_copy(arrow_0,force_result);
            if(negative)
               ULK_vector_2d_mul(arrow_0,arrow_0,-20.0f);
            else
               ULK_vector_2d_mul(arrow_0,arrow_0,20.0f);
            ULK_vector_2d_copy(arrow_1,arrow_0);
            ULK_vector_2d_rot(arrow_0,arrow_0,(ULK_vector_2d){0.0f,0.0f},2.617994f);
            SLK_draw_rgb_line(pos[0],pos[1],pos[0]+arrow_0[0],pos[1]+arrow_0[1],shapes[shape].circle.color);
            ULK_vector_2d_rot(arrow_1,arrow_1,(ULK_vector_2d){0.0f,0.0f},-2.617994f);
            SLK_draw_rgb_line(pos[0],pos[1],pos[0]+arrow_1[0],pos[1]+arrow_1[1],shapes[shape].circle.color);
         }
         ULK_vector_2d_add(pos,pos,force_result);
         SLK_draw_rgb_color((int)pos[0],(int)pos[1],shapes[shape].circle.color);

      }
   }

   putchar('=');
   fflush(stdout);
}

static void draw_shapes()
{
   int i;

   SLK_layer_set_current(1);
   SLK_draw_rgb_set_changed(1);
   
   SLK_draw_rgb_set_clear_color(SLK_color_create(0,0,0,0));
   SLK_draw_rgb_clear();

   for(i = 0;i<shapes_count;i++)
   {
      if(shapes[i].type==0) //Circle, all shapes are calculated as circles, but only real circles are drawn as such
      {
         SLK_draw_rgb_fill_circle(shapes[i].circle.x,shapes[i].circle.y,shapes[i].circle.radius,shapes[i].circle.color);
         SLK_draw_rgb_string(shapes[i].circle.x-72,shapes[i].circle.y-8,2,shapes[i].circle.charge_str,shapes[i].circle.color_inv);
      }
   }

   for(int i = 0;i<rectangles_tmp_count;i++)
   {
      SLK_draw_rgb_fill_rectangle(rectangles_tmp[i].x,rectangles_tmp[i].y,rectangles_tmp[i].width,rectangles_tmp[i].height,rectangles_tmp[i].color);
      SLK_draw_rgb_string(rectangles_tmp[i].x+rectangles_tmp[i].width/2-48,rectangles_tmp[i].y+rectangles_tmp[i].height/2-4,2,rectangles_tmp[i].charge_str,rectangles_tmp[i].color_inv);
   }
}

static void calculate_potential_0()
{
   int part = canvas_width/100;
   for(int x = 0;x<canvas_width;x++)
   {
      for(int y = 0;y<canvas_height;y++)
      {
         ULK_vector_2d point;
         ULK_vector_2d center;
         ULK_vector_2d_set(point,(float)x,(float)y);
         float potential = 0.0f;

         for(int i = 0;i<shapes_count;i++)
         {
            ULK_vector_2d distance;
            float length;
           
            ULK_vector_2d_set(center,(float)shapes[i].circle.x,(float)shapes[i].circle.y);
            ULK_vector_2d_sub(distance,center,point);
            length = ULK_vector_2d_mag(distance);
            
            if(length!=0.0f)
               potential+=8987551788.0f*(shapes[i].circle.charge/length);
         }

         uint8_t color = (uint8_t)(((potential/divider_potential)*255.0f));
         if(color<5)
            SLK_draw_rgb_color(x,y,SLK_color_create(255,255,255,255));
      }
      if(x%part==0)
      {
         putchar('=');
         fflush(stdout);
      }
   }
   puts("");
}

static void calculate_potential_1()
{
   float potential_min;
   float potential_max;

   Circle min = shapes[0].circle;
   Circle max = shapes[0].circle;
   for(int i = 0;i<shapes_count;i++)
   {
      if(shapes[i].circle.charge>max.charge)
         max = shapes[i].circle;
      if(shapes[i].circle.charge<min.charge)
         min = shapes[i].circle;
   }

   for(int i = 0;i<rectangles_tmp_count;i++)
   {
      int rad = rectangles_tmp[i].width>rectangles_tmp[i].height?rectangles_tmp[i].height:rectangles_tmp[i].width;
      if(rectangles_tmp[i].charge>max.charge)
      {
         max.charge = rectangles_tmp[i].charge;
         max.radius = rad;
      }
      if(rectangles_tmp[i].charge<min.charge)
      {
         min.charge = rectangles_tmp[i].charge;
         min.radius = rad;
      }
   }

   if(!manual_potential)
   {
      potential_min = 8987551788.0f*(min.charge/min.radius);
      potential_max = 8987551788.0f*(max.charge/max.radius);
   }
   else
   {
      potential_min = 8987551788.0f*(charge_min/min.radius);
      potential_max = 8987551788.0f*(charge_max/max.radius);
   }

   if(min.charge>0.0f)
      potential_min = 0.0f;
   if(max.charge<0.0f)
      potential_max = 0.0f;

   SLK_draw_rgb_set_target(key_potential);

   for(int y = 0;y<256;y++)
   {
      for(int x = 0;x<256;x++)
      {
         float t = (float)y/255.0f;

         SLK_Color o;
         SLK_Color a;;
         SLK_Color b;
         if(t<0.5)
         {
            t*=2.0f; 
            a = SLK_color_create(0,0,255,255);
            b = SLK_color_create(0,255,0,255);
         }
         else
         {
            t-=0.5f;
            t*=2.0f;
            a = SLK_color_create(0,255,0,255);
            b = SLK_color_create(255,0,0,255);
         }
         o.r = a.r+t*(b.r-a.r);
         o.g = a.g+t*(b.g-a.g);
         o.b = a.b+t*(b.b-a.b);
         o.a = 255;

         SLK_draw_rgb_color(x,y,o);
      }
   }
   char buffer[256];
   sprintf(buffer,"%f",potential_min);
   SLK_draw_rgb_string(0,0,2,buffer,SLK_color_create(255,255,255,255));
   sprintf(buffer,"%f",potential_min+0.5f*(potential_max-potential_min));
   SLK_draw_rgb_string(0,120,2,buffer,SLK_color_create(255,255,255,255));
   sprintf(buffer,"%f",potential_max);
   SLK_draw_rgb_string(0,240,2,buffer,SLK_color_create(255,255,255,255));
   SLK_draw_rgb_set_target(NULL);

   int part = canvas_width/100;
   for(int x = 0;x<canvas_width;x++)
   {
      for(int y = 0;y<canvas_height;y++)
      {
         ULK_vector_2d point;
         ULK_vector_2d center;
         ULK_vector_2d_set(point,(float)x,(float)y);
         float potential = 0.0f;

         for(int i = 0;i<shapes_count;i++)
         {
            ULK_vector_2d distance;
            float length;
           
            ULK_vector_2d_set(center,(float)shapes[i].circle.x,(float)shapes[i].circle.y);
            ULK_vector_2d_sub(distance,center,point);
            length = ULK_vector_2d_mag(distance);
            
            if(length!=0.0f)
               potential+=(8987551788.0f*(shapes[i].circle.charge/length));
         }
         
         float t = (potential-potential_min)/(potential_max-potential_min);
         if(t<0.0f||t>1.0f)
         {
            if(t>1.0f)
               SLK_draw_rgb_color(x,y,SLK_color_create(255,0,0,255));
            else
               SLK_draw_rgb_color(x,y,SLK_color_create(0,0,255,255));
         }
         else 
         {
            SLK_Color o;
            SLK_Color a;;
            SLK_Color b;
            if(t<0.5)
            {
               t*=2.0f; 
               a = SLK_color_create(0,0,255,255);
               b = SLK_color_create(0,255,0,255);
            }
            else
            {
               t-=0.5f;
               t*=2.0f;
               a = SLK_color_create(0,255,0,255);
               b = SLK_color_create(255,0,0,255);
            }
            o.r = a.r+t*(b.r-a.r);
            o.g = a.g+t*(b.g-a.g);
            o.b = a.b+t*(b.b-a.b);
            o.a = 255;

            SLK_draw_rgb_color(x,y,o);
         }
         
      }
      if(x%part==0)
      {
         putchar('=');
         fflush(stdout);
      }
   }
   puts("");
}
//-------------------------------------
