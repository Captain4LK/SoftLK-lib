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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../include/SLK/SLK.h"
//-------------------------------------

//Internal includes
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
typedef struct
{
    int x;
    int y;
    int type;
    int frame;
    int flip;
}Entity;

typedef struct
{
    Entity *entities;
    int used;
    int space;
}Entities;
//-------------------------------------

//Variables
static Entities world;
static SLK_Pal_sprite *dino_sprites[10][4];
static SLK_RGB_sprite *dino_sprites_rgb[10][4];
static SLK_RGB_sprite *gui_00;
static SLK_RGB_sprite *gui_01;
static int frame = 0;
static char time_stat[48];
static int render_mode = 0;
static double timef = 0.1f;
//-------------------------------------

//Function prototypes
static void add_entity();
static void load_pal_sprites();
static void load_rgb_sprites();
static int sort(const void *e0, const void *e1);
static void main_loop();
//-------------------------------------

//Function implementations

int main(int argc, char *argv[])
{
   SLK_setup(320,240,5,"SoftLK - performance test",1,SLK_WINDOW_MAX,0);
   SLK_draw_rgb_load_font("../../data/fonts/cursive1.png");
   SLK_timer_set_fps(30);

   //Layer 0 --> gui
   SLK_layer_create(0,SLK_LAYER_RGB);
   SLK_layer_activate(0,1);
   SLK_layer_set_current(0);
   SLK_draw_rgb_set_clear_color(SLK_color_create(255,255,255,0));
   SLK_draw_rgb_clear();
   gui_00 = SLK_rgb_sprite_load("assets/gui_00.png");
   gui_01 = SLK_rgb_sprite_load("assets/gui_01.png");
   SLK_RGB_sprite *gui_02 = SLK_rgb_sprite_load("assets/gui_02.png");
   SLK_draw_rgb_sprite(gui_02,0,220);
   SLK_draw_rgb_sprite(gui_01,206,2);
   SLK_draw_rgb_string(214,10,1,"pal renderer",SLK_color_create(255,255,255,255));
   SLK_rgb_sprite_destroy(gui_02);
   SLK_draw_rgb_set_changed(1);

   //Layer 1 --> palette sprite drawing
   SLK_layer_create(1,SLK_LAYER_PAL);
   SLK_layer_activate(1,1);
   SLK_layer_set_current(1);
   SLK_draw_pal_set_clear_index(0);
   SLK_draw_pal_clear();

   //Layer 2 --> rgb sprite drawing
   SLK_layer_create(2,SLK_LAYER_RGB);
   SLK_layer_activate(2,1);
   SLK_layer_set_current(2);
   SLK_draw_rgb_set_clear_color(SLK_color_create(255,255,255,0));
   SLK_draw_rgb_clear();
   SLK_draw_rgb_set_changed(1);

   //Layer 4 --> gui
   SLK_layer_create(4,SLK_LAYER_RGB);
   SLK_layer_activate(4,1);
   SLK_layer_set_current(4);
   SLK_RGB_sprite *gui_03 = SLK_rgb_sprite_load("assets/gui_03.png");
   SLK_draw_rgb_set_changed(1);
   SLK_draw_rgb_sprite(gui_03,0,0);
   SLK_rgb_sprite_destroy(gui_03);

   //Load sprites
   load_pal_sprites();
   load_rgb_sprites();

   //World setup
   srand(time(NULL));
   world.space = 100;
   world.used = 0;
   world.entities= malloc(sizeof(Entity)*world.space);
   while(world.used<1000)
      add_entity();
      
   //Main loop
   while(SLK_core_running())
   {
      main_loop();
   }

   return 0;
}

static void main_loop()
{
   SLK_update();
   frame++;

   if(render_mode==0)
   {
      SLK_layer_set_current(1);
      SLK_draw_pal_clear();
   }
   else if(render_mode==1)
   {
      SLK_layer_set_current(2);
      SLK_draw_rgb_clear();
      SLK_draw_rgb_set_changed(1);
   }
   
   //Sprite animation, drawing and benchmark
   int next_frame = frame%4==0;
   clock_t start = clock();
   for(int i = 0;i<world.used;i++)
   {
      if(next_frame)
      {
         world.entities[i].frame++;
         if(world.entities[i].frame>3)
            world.entities[i].frame = 0;
      }

      if(render_mode==0)
         SLK_draw_pal_sprite_flip(dino_sprites[world.entities[i].type][world.entities[i].frame],world.entities[i].x,world.entities[i].y,world.entities[i].flip);
      else if(render_mode==1)
         SLK_draw_rgb_sprite_flip(dino_sprites_rgb[world.entities[i].type][world.entities[i].frame],world.entities[i].x,world.entities[i].y,world.entities[i].flip);
   }
   timef+=((double)(clock()-start)/CLOCKS_PER_SEC);
   if(next_frame)
      sprintf(time_stat,"%08lf %04d",timef/((double)frame),world.used);
   //-------------------------------------

   //Input
   if((SLK_key_down(SLK_KEY_SPACE)||SLK_gamepad_down(0,SLK_PAD_A))&&world.used<9999)
      add_entity();

   if(SLK_key_pressed(SLK_KEY_P)||SLK_gamepad_pressed(0,SLK_PAD_LEFTSHOULDER))
   {
      render_mode = 0;
      timef = 0.0f;
      frame = 0;

      SLK_layer_activate(2,0);
      SLK_layer_activate(1,1);

      SLK_layer_set_current(0);
      SLK_draw_rgb_set_changed(1);
      SLK_draw_rgb_sprite(gui_01,206,2);
      SLK_draw_rgb_string(214,10,1,"pal renderer",SLK_color_create(255,255,255,255));
   }
   else if(SLK_key_pressed(SLK_KEY_R)||SLK_gamepad_pressed(0,SLK_PAD_RIGHTSHOULDER))
   {
      render_mode = 1;
      timef = 0.0f;
      frame = 0;

      SLK_layer_activate(2,1);
      SLK_layer_activate(1,0);

      SLK_layer_set_current(0);
      SLK_draw_rgb_set_changed(1);
      SLK_draw_rgb_sprite(gui_01,206,2);
      SLK_draw_rgb_string(214,10,1,"rgb renderer",SLK_color_create(255,255,255,255));
   }
   /*if(SLK_key_pressed(SLK_KEY_F))
   {
      fullscreen = !fullscreen;
      SLK_core_set_fullscreen(fullscreen);
   }*/
   //-------------------------------------
   
   //Update gui
   SLK_layer_set_current(0);
   SLK_draw_rgb_set_changed(1);
   SLK_draw_rgb_sprite(gui_00,2,2);
   SLK_draw_rgb_string(10,10,1,time_stat,SLK_color_create(255,255,255,255));

   SLK_render_update();
}

static void add_entity()
{
   world.entities[world.used].frame = rand()%4;
   world.entities[world.used].x = rand()%294+5;
   world.entities[world.used].y = rand()%201+10;
   world.entities[world.used].type = rand()%10;
   world.entities[world.used].flip = (rand()%2)?SLK_FLIP_HORIZONTAL:SLK_FLIP_NONE;

   world.used++;

   qsort(world.entities,world.used,sizeof(Entity),sort);

   if(world.used>=world.space)
   {
      world.space+=100;
      world.entities = realloc(world.entities,sizeof(Entity)*world.space);
   }
}

static void load_pal_sprites()
{
   SLK_layer_set_palette(1,SLK_palette_load("assets/palette.pal"));

   SLK_Pal_sprite *tileset = SLK_pal_sprite_load("assets/sprites.slk");

   for(int y = 0;y<10;y++)
   {
      for(int x = 0;x<4;x++)
      {
         dino_sprites[y][x] = SLK_pal_sprite_create(16,24);
         SLK_pal_sprite_copy_partial(dino_sprites[y][x],tileset,0,0,x*16,y*24,16,24);
      }
   }

   SLK_pal_sprite_destroy(tileset);
}

static void load_rgb_sprites()
{
   SLK_RGB_sprite *tileset = SLK_rgb_sprite_load("assets/sprites.png");

   for(int y = 0;y<10;y++)
   {
      for(int x = 0;x<4;x++)
      {
         dino_sprites_rgb[y][x] = SLK_rgb_sprite_create(16,24);
         SLK_rgb_sprite_copy_partial(dino_sprites_rgb[y][x],tileset,0,0,x*16,y*24,16,24);
      }
   }

   SLK_rgb_sprite_destroy(tileset);
}

static int sort(const void *e0, const void *e1)
{
   return ((Entity *)e0)->y - ((Entity *)e1)->y;
}
//-------------------------------------
