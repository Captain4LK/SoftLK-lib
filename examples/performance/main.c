#include "../../include/SLK/SLK.h"

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

Entities world;
SLK_Pal_sprite *dino_sprites[10][4];
SLK_RGB_sprite *dino_sprites_rgb[10][4];
SLK_RGB_sprite *gui_00;
SLK_RGB_sprite *gui_01;
int frame = 0;
char time_stat[48];
int render_mode = 0;
int fullscreen = 0;

void add_entity();
void load_pal_sprites();
void load_rgb_sprites();
int sort(const void *e0, const void *e1);

int main(int argc, char *argv[])
{
   SLK_setup(320,240,5,"SLK Engine",0,SLK_WINDOW_MAX);
   SLK_timer_set_fps(30);
   srand(time(NULL));

   SLK_layer_create(0,SLK_LAYER_RGB);
   SLK_layer_create(1,SLK_LAYER_PAL);
   SLK_layer_create(2,SLK_LAYER_RGB);
   SLK_layer_create(4,SLK_LAYER_RGB);

   SLK_layer_activate(0,1);
   SLK_layer_activate(1,1);
   SLK_layer_activate(2,1);
   SLK_layer_activate(4,1);

   SLK_layer_set_current(0);
   SLK_layer_set_current(1);

   SLK_layer_set_tint(1,SLK_color_create(255,255,255,128));
   SLK_layer_set_tint(2,SLK_color_create(255,255,255,128));

   SLK_draw_pal_set_clear_paxel(SLK_color_create_paxel(0,SLK_TRANSPARENT));
   SLK_draw_pal_clear();
   SLK_draw_rgb_set_clear_color(SLK_color_create(255,255,255,0));
   SLK_draw_rgb_clear();

   load_pal_sprites();
   load_rgb_sprites();

   gui_00 = SLK_rgb_sprite_load("assets/gui_00.png");
   gui_01 = SLK_rgb_sprite_load("assets/gui_01.png");
   SLK_RGB_sprite *gui_02 = SLK_rgb_sprite_load("assets/gui_02.png");
   SLK_layer_set_current(0);
   SLK_draw_rgb_sprite(gui_02,0,220);
   SLK_draw_rgb_sprite(gui_01,206,2);
   SLK_draw_rgb_string(214,10,1,"pal renderer",SLK_color_create(255,255,255,255));
   SLK_rgb_sprite_destroy(gui_02);
   SLK_RGB_sprite *gui_03 = SLK_rgb_sprite_load("assets/gui_03.png");
   SLK_layer_set_current(4);
   SLK_draw_rgb_sprite(gui_03,0,0);
   SLK_rgb_sprite_destroy(gui_03);

   world.space = 100;
   world.used = 0;
   world.entities= malloc(sizeof(Entity)*world.space);
   double time = 0.1f;
   
   while(world.used<10)
      add_entity();
      
   while(SLK_core_running())
   {
      SLK_update();

      if(render_mode==0)
      {
         SLK_layer_set_current(1);
         SLK_draw_pal_clear();
      }
      else if(render_mode==1)
      {
         SLK_layer_set_current(2);
         SLK_draw_rgb_clear();
      }

      frame++;
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
         {
            SLK_draw_pal_sprite_flip(dino_sprites[world.entities[i].type][world.entities[i].frame],world.entities[i].x,world.entities[i].y,world.entities[i].flip?SLK_FLIP_HORIZONTAL:SLK_FLIP_NONE);
         }
         else if(render_mode==1)
         {
            SLK_draw_rgb_sprite_flip(dino_sprites_rgb[world.entities[i].type][world.entities[i].frame],world.entities[i].x,world.entities[i].y,world.entities[i].flip?SLK_FLIP_HORIZONTAL:SLK_FLIP_NONE);
         }
      }
      time+=((double)(clock()-start)/CLOCKS_PER_SEC);
      if(next_frame)
         sprintf(time_stat,"%08lf %04d",time/((double)frame)/*/world.used*10.0f*/,world.used);
 
      if(SLK_key_down(SLK_KEY_SPACE)&&world.used<9999)
         add_entity();

      if(SLK_key_pressed(SLK_KEY_P))
      {
         render_mode = 0;
         time = 0.0f;
         frame = 0;

         SLK_layer_activate(2,0);
         SLK_layer_activate(1,1);

         SLK_layer_set_current(0);
         SLK_draw_rgb_sprite(gui_01,206,2);
         SLK_draw_rgb_string(214,10,1,"pal renderer",SLK_color_create(255,255,255,255));
      }
      if(SLK_key_pressed(SLK_KEY_R))
      {
         render_mode = 1;
         time = 0.0f;
         frame = 0;

         SLK_layer_activate(2,1);
         SLK_layer_activate(1,0);

         SLK_layer_set_current(0);
         SLK_draw_rgb_sprite(gui_01,206,2);
         SLK_draw_rgb_string(214,10,1,"rgb renderer",SLK_color_create(255,255,255,255));
      }
      if(SLK_key_pressed(SLK_KEY_F))
      {
         fullscreen = !fullscreen;
         SLK_core_set_fullscreen(fullscreen);
      }
      
      
      SLK_layer_set_current(0);
      SLK_draw_rgb_sprite(gui_00,2,2);
      SLK_draw_rgb_string(10,10,1,time_stat,SLK_color_create(255,255,255,255));

      SLK_render_update();
   }

   return 0;
}

void add_entity()
{
   world.entities[world.used].frame = rand()%4;
   world.entities[world.used].x = rand()%294+5;
   world.entities[world.used].y = rand()%201+10;
   world.entities[world.used].type = rand()%10;
   world.entities[world.used].flip = rand()%2;

   world.used++;

   qsort(world.entities,world.used,sizeof(Entity),sort);

   if(world.used>=world.space)
   {
      world.space+=100;
      world.entities = realloc(world.entities,sizeof(Entity)*world.space);
   }
}

void load_pal_sprites()
{
   SLK_layer_set_current(1);
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

void load_rgb_sprites()
{
   SLK_layer_set_current(2);

   SLK_RGB_sprite *tileset = SLK_rgb_sprite_load("assets/sprites.png");

   for(int y = 0;y<10;y++)
   {
      for(int x = 0;x<4;x++)
      {
         dino_sprites_rgb[y][x] = SLK_rgb_sprite_create(16,24);
         SLK_draw_rgb_set_target(dino_sprites_rgb[y][x]);
         SLK_draw_rgb_clear();
         SLK_draw_rgb_sprite_partial(tileset,0,0,x*16,y*24,16,24);
      }
   }
   SLK_rgb_sprite_destroy(tileset);
}

int sort(const void *e0, const void *e1)
{
   if(((Entity *)e0)->y<((Entity *)e1)->y)
      return -1;
   else if(((Entity *)e0)->y>((Entity *)e1)->y)
      return 1;
   else
      return 0;
}
