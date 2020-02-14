/*
  Copyright (C) 2020 Captain4LK (Lukas Holzbeierlein)

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Captain4LK
  commandant4lk@gmail.com

*/

#ifndef _ULK_VECTOR_H_

#define _ULK_VECTOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef float ULK_vector_2d[2];
typedef float ULK_vector_3d[3];
typedef float ULK_vector_4d[4];

void ULK_vector_2d_add(ULK_vector_2d out, const ULK_vector_2d a, const ULK_vector_2d b);
float ULK_vector_2d_angle(const ULK_vector_2d a, const ULK_vector_2d b);
void ULK_vector_2d_ceil(ULK_vector_2d out, const ULK_vector_2d in);
ULK_vector_2d *ULK_vector_2d_clone(const ULK_vector_2d in);
void ULK_vector_2d_copy(ULK_vector_2d dst, const ULK_vector_2d src);
ULK_vector_2d *ULK_vector_2d_create();
void ULK_vector_2d_cross(ULK_vector_3d out, const ULK_vector_2d a, const ULK_vector_2d b);
float ULK_vector_2d_distance(const ULK_vector_2d a, const ULK_vector_2d b);
void ULK_vector_2d_divide(ULK_vector_2d out, const ULK_vector_2d a, const ULK_vector_2d b);
float ULK_vector_2d_dot(const ULK_vector_2d a, const ULK_vector_2d b);
int ULK_vector_2d_equals(const ULK_vector_2d a, const ULK_vector_2d b);
int ULK_vector_2d_exact_equals(const ULK_vector_2d a, const ULK_vector_2d b);
void ULK_vector_2d_floor(ULK_vector_2d out, const ULK_vector_2d in);
ULK_vector_2d *ULK_vector_2d_from_values(const float x, const float y);
void ULK_vector_2d_invert(ULK_vector_2d out, const ULK_vector_2d in);
float ULK_vector_2d_length(const ULK_vector_2d in);
void ULK_vector_2d_lerp(ULK_vector_2d out, const ULK_vector_2d a, const ULK_vector_2d b, const float t);
void ULK_vector_2d_max(ULK_vector_2d out, const ULK_vector_2d a, const ULK_vector_2d b);
void ULK_vector_2d_min(ULK_vector_2d out, const ULK_vector_2d a, const ULK_vector_2d b);
void ULK_vector_2d_multiply(ULK_vector_2d out, const ULK_vector_2d a, const ULK_vector_2d b);
void ULK_vector_2d_negate(ULK_vector_2d out, const ULK_vector_2d in);
void ULK_vector_2d_normalize(ULK_vector_2d out, const ULK_vector_2d in);
void ULK_vector_2d_rotate(ULK_vector_2d out, const ULK_vector_2d in, const ULK_vector_2d origin, const float radians);
void ULK_vector_2d_round(ULK_vector_2d out, const ULK_vector_2d in);
void ULK_vector_2d_scale(ULK_vector_2d out, const ULK_vector_2d in, const float scale);
void ULK_vector_2d_set(ULK_vector_2d out, const float x, const float y);
float ULK_vector_2d_squared_distance(const ULK_vector_2d a, const ULK_vector_2d b);
float ULK_vector_2d_squared_length(const ULK_vector_2d in);
void ULK_vector_2d_subtract(ULK_vector_2d out, const ULK_vector_2d a, const ULK_vector_2d b);
void ULK_vector_2d_zero(ULK_vector_2d out);

void ULK_vector_3d_add(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b);
float ULK_vector_3d_angle(const ULK_vector_3d a, const ULK_vector_3d b);
void ULK_vector_3d_bezier(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b, const ULK_vector_3d c, const ULK_vector_3d d, const float t);
void ULK_vector_3d_ceil(ULK_vector_3d out, const ULK_vector_3d in);
ULK_vector_3d *ULK_vector_3d_clone(const ULK_vector_3d in);
void ULK_vector_3d_copy(ULK_vector_3d dst, const ULK_vector_3d src);
ULK_vector_3d *ULK_vector_3d_create();
void ULK_vector_3d_cross(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b);
float ULK_vector_3d_distance(const ULK_vector_3d a, const ULK_vector_3d b);
void ULK_vector_3d_divide(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b);
float ULK_vector_3d_dot(const ULK_vector_3d a, const ULK_vector_3d b);
int ULK_vector_3d_equals(const ULK_vector_3d a, const ULK_vector_3d b);
int ULK_vector_3d_exact_equals(const ULK_vector_3d a, const ULK_vector_3d b);
void ULK_vector_3d_floor(ULK_vector_3d out, const ULK_vector_3d in);
ULK_vector_3d *ULK_vector_3d_from_values(const float x, const float y, const float z);
void ULK_vector_3d_hermite(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b, const ULK_vector_3d c, const ULK_vector_3d d, float t);
void ULK_vector_3d_inverse(ULK_vector_3d out, const ULK_vector_3d in);
float ULK_vector_3d_length(const ULK_vector_3d in);
void ULK_vector_3d_lerp(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b, const float t);
void ULK_vector_3d_max(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b);
void ULK_vector_3d_min(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b);
void ULK_vector_3d_multiply(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b);
void ULK_vector_3d_negate(ULK_vector_3d out, const ULK_vector_3d in);
void ULK_vector_3d_normalize(ULK_vector_3d out, const ULK_vector_3d in);
void ULK_vector_3d_rotate_x(ULK_vector_3d out, const ULK_vector_3d in, const ULK_vector_3d origin, const float rad);
void ULK_vector_3d_rotate_y(ULK_vector_3d out, const ULK_vector_3d in, const ULK_vector_3d origin, const float rad);
void ULK_vector_3d_rotate_z(ULK_vector_3d out, const ULK_vector_3d in, const ULK_vector_3d origin, const float rad);
void ULK_vector_3d_round(ULK_vector_3d out, const ULK_vector_3d in);
void ULK_vector_3d_scale(ULK_vector_3d out, const ULK_vector_3d in, const float scale);
void ULK_vector_3d_set(ULK_vector_3d out, const float x, const float y, const float z);
float ULK_vector_3d_squared_distance(const ULK_vector_3d a, const ULK_vector_3d b);
float ULK_vector_3d_squared_length(const ULK_vector_3d in);
void ULK_vector_3d_subtract(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b);
void ULK_vector_3d_zero(ULK_vector_3d out);

void ULK_vector_4d_add(ULK_vector_4d out, const ULK_vector_4d a, const ULK_vector_4d b);
void ULK_vector_4d_ceil(ULK_vector_4d out, const ULK_vector_4d in);
ULK_vector_4d *ULK_vector_4d_clone(const ULK_vector_4d in);
void ULK_vector_4d_copy(ULK_vector_4d out, const ULK_vector_4d in);
ULK_vector_4d *ULK_vector_4d_create();
void ULK_vector_4d_cross(ULK_vector_4d out, const ULK_vector_4d a, const ULK_vector_4d b, const ULK_vector_4d c);
float ULK_vector_4d_distance(const ULK_vector_4d a, const ULK_vector_4d b);
void ULK_vector_4d_divide(ULK_vector_4d out, const ULK_vector_4d a, const ULK_vector_4d b);
float ULK_vector_4d_dot(const ULK_vector_4d a, const ULK_vector_4d b);
int ULK_vector_4d_equals(const ULK_vector_4d a, const ULK_vector_4d b);
int ULK_vector_4d_exact_equals(const ULK_vector_4d a, const ULK_vector_4d b);
void ULK_vector_4d_floor(ULK_vector_4d out, const ULK_vector_4d in);
ULK_vector_4d *ULK_vector_4d_from_values(const float x, const float y, const float z, const float w);
void ULK_vector_4d_inverse(ULK_vector_4d out, const ULK_vector_4d in);
float ULK_vector_4d_length(const ULK_vector_4d in);
void ULK_vector_4d_lerp(ULK_vector_4d out, const ULK_vector_4d a, const ULK_vector_4d b, const float t);
void ULK_vector_4d_max(ULK_vector_4d out, const ULK_vector_4d a, const ULK_vector_4d b);
void ULK_vector_4d_min(ULK_vector_4d out, const ULK_vector_4d a, const ULK_vector_4d b);
void ULK_vector_4d_multiply(ULK_vector_4d out, const ULK_vector_4d a, const ULK_vector_4d b);
void ULK_vector_4d_negate(ULK_vector_4d out, const ULK_vector_4d in);
void ULK_vector_4d_normalize(ULK_vector_4d out, const ULK_vector_4d in);
void ULK_vector_4d_round(ULK_vector_4d out, const ULK_vector_4d in);
void ULK_vector_4d_scale(ULK_vector_4d out, const ULK_vector_4d in, const float scale);
void ULK_vector_4d_set(ULK_vector_4d out, const float x, const float y, const float z, const float w);
float ULK_vector_4d_squared_distance(const ULK_vector_4d a, const ULK_vector_4d b);
float ULK_vector_4d_squared_length(const ULK_vector_4d in);
void ULK_vector_4d_subtract(ULK_vector_4d out, const ULK_vector_4d a, const ULK_vector_4d b);
void ULK_vector_4d_zero(ULK_vector_4d out);

#endif
