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

#ifndef _SETTINGS_H_

#define _SETTINGS_H_

#include "../../include/SLK/SLK_types.h"

extern SLK_Color background_color;
extern int canvas_width;
extern int canvas_height;
extern float gui_scale;

void settings_init_default();

#endif 
