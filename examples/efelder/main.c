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

//External includes
#include "../../include/SLK/SLK.h"
#include "../../external/UtilityLK/include/ULK_vector.h"
//-------------------------------------

//Internal includes
#include "gui.h"
#include "settings.h"
#include "calculate.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
//-------------------------------------

//Function prototypes
void draw_shapes();
//-------------------------------------

//Function implementations

int main(int argc, char *argv[])
{
   if(argc<2)
      return -1;
   settings_init_default();
   shapes_load_file(argv[1]);

   SLK_setup(800,600,6,"Efelder",0,1,1);
   SLK_core_set_visible(0);
   SLK_timer_set_fps(30);

   SLK_layer_create(0,SLK_LAYER_RGB); //Layer for GUI
   SLK_layer_create(1,SLK_LAYER_RGB); //Layer for shapes
   SLK_layer_create(2,SLK_LAYER_RGB); //Layer for drawing electric field
   SLK_layer_create(3,SLK_LAYER_RGB); //Layer for drawing electric potential
   SLK_layer_create(4,SLK_LAYER_RGB); //Layer for editing layout
   SLK_layer_create(5,SLK_LAYER_RGB); //Layer for background

   SLK_layer_activate(0,1);
   SLK_layer_set_dynamic(0,0);
   SLK_layer_activate(1,1);
   SLK_layer_set_dynamic(1,0);
   SLK_layer_activate(2,0);
   SLK_layer_set_dynamic(2,0);
   SLK_layer_activate(3,1);
   SLK_layer_set_dynamic(3,0);
   SLK_layer_activate(4,0);
   SLK_layer_set_dynamic(4,0);
   SLK_layer_activate(5,1);
   SLK_layer_set_dynamic(5,1);

   SLK_draw_rgb_load_font("../../data/fonts/cursive2.png");
   key_potential = SLK_rgb_sprite_create(256,256);

   gui_init();

   calculate();

   while(SLK_core_running())
   {
      SLK_update();
      gui_update();

      gui_draw();
      SLK_render_update();
   }

   return 0;
}


//-------------------------------------
