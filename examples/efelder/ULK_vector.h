/*

██    ██ ██      ██   ██        ██    ██ ███████  ██████ ████████  ██████  ██████  
██    ██ ██      ██  ██         ██    ██ ██      ██         ██    ██    ██ ██   ██ 
██    ██ ██      █████          ██    ██ █████   ██         ██    ██    ██ ██████  
██    ██ ██      ██  ██          ██  ██  ██      ██         ██    ██    ██ ██   ██ 
 ██████  ███████ ██   ██ ███████  ████   ███████  ██████    ██     ██████  ██   ██ 
                                                                                   
*/

/* 
LICENSE OPTION A: 3-clause BSD

   Copyright (C) 2020 Captain4LK (Lukas Holzbeierlein)

   Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

   3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

LICENSE OPTION B: Public Domain CC0

   just credit the original creator of gl-matrix as stated below.
*/

/*
Based on gl-matrix: https://github.com/toji/gl-matrix 

Original license notice:

Copyright (c) 2015-2020, Brandon Jones, Colin MacKenzie IV.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*

██    ██ ███████  █████   ██████  ███████ 
██    ██ ██      ██   ██ ██       ██      
██    ██ ███████ ███████ ██   ███ █████   
██    ██      ██ ██   ██ ██    ██ ██      
 ██████  ███████ ██   ██  ██████  ███████ 
                                          
-----------------------------------------

1. Add ULK_vector.c to your source files.
2. Add ULK_vector.h to your include files.
3. Edit ULK_vector.c, l.43 to match the path to ULK_vector.h

Linker options: None, unless your plattform treats math.h as a library, then add -lm (eg. Linux).
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
void ULK_vector_2d_copy(ULK_vector_2d dst, const ULK_vector_2d src);
void ULK_vector_2d_cross(ULK_vector_3d out, const ULK_vector_2d a, const ULK_vector_2d b);
float ULK_vector_2d_distance(const ULK_vector_2d a, const ULK_vector_2d b);
void ULK_vector_2d_divide(ULK_vector_2d out, const ULK_vector_2d a, const ULK_vector_2d b);
float ULK_vector_2d_dot(const ULK_vector_2d a, const ULK_vector_2d b);
int ULK_vector_2d_equals(const ULK_vector_2d a, const ULK_vector_2d b);
int ULK_vector_2d_exact_equals(const ULK_vector_2d a, const ULK_vector_2d b);
void ULK_vector_2d_floor(ULK_vector_2d out, const ULK_vector_2d in);
void ULK_vector_2d_invert(ULK_vector_2d out, const ULK_vector_2d in);
float ULK_vector_2d_length(const ULK_vector_2d in);
void ULK_vector_2d_lerp(ULK_vector_2d out, const ULK_vector_2d a, const ULK_vector_2d b, float t);
void ULK_vector_2d_max(ULK_vector_2d out, const ULK_vector_2d a, const ULK_vector_2d b);
void ULK_vector_2d_min(ULK_vector_2d out, const ULK_vector_2d a, const ULK_vector_2d b);
void ULK_vector_2d_multiply(ULK_vector_2d out, const ULK_vector_2d a, const ULK_vector_2d b);
void ULK_vector_2d_negate(ULK_vector_2d out, const ULK_vector_2d in);
void ULK_vector_2d_normalize(ULK_vector_2d out, const ULK_vector_2d in);
void ULK_vector_2d_rotate(ULK_vector_2d out, const ULK_vector_2d in, const ULK_vector_2d origin, float radians);
void ULK_vector_2d_round(ULK_vector_2d out, const ULK_vector_2d in);
void ULK_vector_2d_scale(ULK_vector_2d out, const ULK_vector_2d in, float scale);
void ULK_vector_2d_set(ULK_vector_2d out, float x, float y);
float ULK_vector_2d_squared_distance(const ULK_vector_2d a, const ULK_vector_2d b);
float ULK_vector_2d_squared_length(const ULK_vector_2d in);
void ULK_vector_2d_subtract(ULK_vector_2d out, const ULK_vector_2d a, const ULK_vector_2d b);
void ULK_vector_2d_zero(ULK_vector_2d out);

void ULK_vector_3d_add(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b);
float ULK_vector_3d_angle(const ULK_vector_3d a, const ULK_vector_3d b);
void ULK_vector_3d_bezier(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b, const ULK_vector_3d c, const ULK_vector_3d d, const float t);
void ULK_vector_3d_ceil(ULK_vector_3d out, const ULK_vector_3d in);
void ULK_vector_3d_copy(ULK_vector_3d dst, const ULK_vector_3d src);
void ULK_vector_3d_cross(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b);
float ULK_vector_3d_distance(const ULK_vector_3d a, const ULK_vector_3d b);
void ULK_vector_3d_divide(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b);
float ULK_vector_3d_dot(const ULK_vector_3d a, const ULK_vector_3d b);
int ULK_vector_3d_equals(const ULK_vector_3d a, const ULK_vector_3d b);
int ULK_vector_3d_exact_equals(const ULK_vector_3d a, const ULK_vector_3d b);
void ULK_vector_3d_floor(ULK_vector_3d out, const ULK_vector_3d in);
void ULK_vector_3d_hermite(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b, const ULK_vector_3d c, const ULK_vector_3d d, float t);
void ULK_vector_3d_inverse(ULK_vector_3d out, const ULK_vector_3d in);
float ULK_vector_3d_length(const ULK_vector_3d in);
void ULK_vector_3d_lerp(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b, float t);
void ULK_vector_3d_max(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b);
void ULK_vector_3d_min(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b);
void ULK_vector_3d_multiply(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b);
void ULK_vector_3d_negate(ULK_vector_3d out, const ULK_vector_3d in);
void ULK_vector_3d_normalize(ULK_vector_3d out, const ULK_vector_3d in);
void ULK_vector_3d_rotate_x(ULK_vector_3d out, const ULK_vector_3d in, const ULK_vector_3d origin, float rad);
void ULK_vector_3d_rotate_y(ULK_vector_3d out, const ULK_vector_3d in, const ULK_vector_3d origin, float rad);
void ULK_vector_3d_rotate_z(ULK_vector_3d out, const ULK_vector_3d in, const ULK_vector_3d origin, float rad);
void ULK_vector_3d_round(ULK_vector_3d out, const ULK_vector_3d in);
void ULK_vector_3d_scale(ULK_vector_3d out, const ULK_vector_3d in, float scale);
void ULK_vector_3d_set(ULK_vector_3d out, float x, float y, float z);
void ULK_vector_3d_slerp(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b, float t);
float ULK_vector_3d_squared_distance(const ULK_vector_3d a, const ULK_vector_3d b);
float ULK_vector_3d_squared_length(const ULK_vector_3d in);
void ULK_vector_3d_subtract(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b);
void ULK_vector_3d_zero(ULK_vector_3d out);

void ULK_vector_4d_add(ULK_vector_4d out, const ULK_vector_4d a, const ULK_vector_4d b);
void ULK_vector_4d_ceil(ULK_vector_4d out, const ULK_vector_4d in);
void ULK_vector_4d_copy(ULK_vector_4d out, const ULK_vector_4d in);
void ULK_vector_4d_cross(ULK_vector_4d out, const ULK_vector_4d a, const ULK_vector_4d b, const ULK_vector_4d c);
float ULK_vector_4d_distance(const ULK_vector_4d a, const ULK_vector_4d b);
void ULK_vector_4d_divide(ULK_vector_4d out, const ULK_vector_4d a, const ULK_vector_4d b);
float ULK_vector_4d_dot(const ULK_vector_4d a, const ULK_vector_4d b);
int ULK_vector_4d_equals(const ULK_vector_4d a, const ULK_vector_4d b);
int ULK_vector_4d_exact_equals(const ULK_vector_4d a, const ULK_vector_4d b);
void ULK_vector_4d_floor(ULK_vector_4d out, const ULK_vector_4d in);
void ULK_vector_4d_inverse(ULK_vector_4d out, const ULK_vector_4d in);
float ULK_vector_4d_length(const ULK_vector_4d in);
void ULK_vector_4d_lerp(ULK_vector_4d out, const ULK_vector_4d a, const ULK_vector_4d b, float t);
void ULK_vector_4d_max(ULK_vector_4d out, const ULK_vector_4d a, const ULK_vector_4d b);
void ULK_vector_4d_min(ULK_vector_4d out, const ULK_vector_4d a, const ULK_vector_4d b);
void ULK_vector_4d_multiply(ULK_vector_4d out, const ULK_vector_4d a, const ULK_vector_4d b);
void ULK_vector_4d_negate(ULK_vector_4d out, const ULK_vector_4d in);
void ULK_vector_4d_normalize(ULK_vector_4d out, const ULK_vector_4d in);
void ULK_vector_4d_round(ULK_vector_4d out, const ULK_vector_4d in);
void ULK_vector_4d_scale(ULK_vector_4d out, const ULK_vector_4d in, float scale);
void ULK_vector_4d_set(ULK_vector_4d out, float x, float y, float z, float w);
float ULK_vector_4d_squared_distance(const ULK_vector_4d a, const ULK_vector_4d b);
float ULK_vector_4d_squared_length(const ULK_vector_4d in);
void ULK_vector_4d_subtract(ULK_vector_4d out, const ULK_vector_4d a, const ULK_vector_4d b);
void ULK_vector_4d_zero(ULK_vector_4d out);

#endif
