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

#ifndef _CALCULATE_H_

#define _CALCULATE_H_

#include "../../include/SLK/SLK.h"

typedef struct
{
   int x;
   int y;
   int radius;
   float charge;
   char charge_str[16];
   int test_points;
   SLK_Color color;
   SLK_Color color_inv;
}Circle;

typedef struct
{
   int x;
   int y;
   int width;
   int height;
   float charge;
   char charge_str[16];
   int test_points;
   SLK_Color color;
   SLK_Color color_inv;
}Rectangle;

typedef struct
{
   int type;

   Circle circle;
}Shape;

Shape *shapes;
int shapes_count;
Rectangle *rectangles_tmp;
int rectangles_tmp_count;

void shapes_load_file(const char *path);
void calculate();

#endif
