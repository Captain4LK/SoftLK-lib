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

#include "ULK_vector.h"

#define EPSILON 0.000001f
#define MAX(a,b) (a>b?a:b)
#define MIN(a,b) (a<b?a:b)

void ULK_vector_2d_add(ULK_vector_2d out, const ULK_vector_2d a, const ULK_vector_2d b)
{
   out[0] = a[0]+b[0];
   out[1] = a[1]+b[1];
}

float ULK_vector_2d_angle(const ULK_vector_2d a, const ULK_vector_2d b)
{
   float cosine = 0.0f;
   float magnitude = sqrtf(a[0]*a[0]+a[1]*a[1])*sqrtf(b[0]*b[0]+b[1]*b[1]);
   
   if(magnitude!=0.0f)
      cosine = (a[0]*b[0]+a[1]*b[1])/magnitude;
   
   return acosf(cosine);
}

void ULK_vector_2d_ceil(ULK_vector_2d out, const ULK_vector_2d in)
{
   out[0] = ceilf(in[0]);
   out[1] = ceilf(in[1]);
}

ULK_vector_2d *ULK_vector_2d_clone(const ULK_vector_2d in)
{
   ULK_vector_2d *out = malloc(sizeof(ULK_vector_2d));
   *out[0] = in[0];
   *out[1] = in[1];
   
   return out;
}

void ULK_vector_2d_copy(ULK_vector_2d dst, const ULK_vector_2d src)
{
   dst[0] = src[0];
   dst[1] = src[1];
}

ULK_vector_2d *ULK_vector_2d_create()
{
   ULK_vector_2d *out = malloc(sizeof(ULK_vector_2d));
   
   *out[0] = 0.0f;
   *out[1] = 0.0f;
      
   return out;
}

void ULK_vector_2d_cross(ULK_vector_3d out, const ULK_vector_2d a, const ULK_vector_2d b)
{
   float z = a[0]*b[1]-a[1]*b[0];
   out[0] = 0;
   out[1] = 0;
   out[2] = z;
}

float ULK_vector_2d_distance(const ULK_vector_2d a, const ULK_vector_2d b)
{
   float x = b[0]-a[0];
   float y = b[1]-a[1];
   
   return hypotf(x,y);
}

void ULK_vector_2d_divide(ULK_vector_2d out, const ULK_vector_2d a, const ULK_vector_2d b)
{
   out[0] = a[0]/b[0];
   out[1] = a[1]/b[1];
}

float ULK_vector_2d_dot(const ULK_vector_2d a, const ULK_vector_2d b)
{
   return a[0]*b[0]+a[1]*b[1];
}

int ULK_vector_2d_equals(const ULK_vector_2d a, const ULK_vector_2d b)
{
   return (fabs(a[0]-b[0])<=(EPSILON*MAX(1.0f,MAX(fabs(a[0]),fabs(b[0]))))&&fabs(a[1]-b[1])<=(EPSILON*MAX(1.0f,MAX(fabs(a[1]),fabs(b[1])))));
}

int ULK_vector_2d_exact_equals(const ULK_vector_2d a, const ULK_vector_2d b)
{
   return (a[0]==b[0]&&a[1]==b[1]);
}

void ULK_vector_2d_floor(ULK_vector_2d out, const ULK_vector_2d in)
{
   out[0] = floorf(in[0]);
   out[1] = floorf(in[1]);
}

ULK_vector_2d *ULK_vector_2d_from_values(const float x, const float y)
{
   ULK_vector_2d *out = malloc(sizeof(ULK_vector_2d));
   *out[0] = x;
   *out[1] = y;
   
   return out;
}

void ULK_vector_2d_invert(ULK_vector_2d out, const ULK_vector_2d in)
{
   out[0] = 1.0f/in[0];
   out[1] = 1.0f/in[1];
}

float ULK_vector_2d_length(const ULK_vector_2d in)
{
   return sqrtf(in[0]*in[0]+in[1]*in[1]);
}

void ULK_vector_2d_lerp(ULK_vector_2d out, const ULK_vector_2d a, const ULK_vector_2d b, const float t)
{
   out[0] = a[0]+t*(b[0]-a[0]);
   out[1] = a[1]+t*(b[1]-a[1]);
}

void ULK_vector_2d_max(ULK_vector_2d out, const ULK_vector_2d a, const ULK_vector_2d b)
{
   float a_length = a[0]*a[0]+a[1]*a[1];
   float b_length = b[0]*b[0]+b[1]*b[1];
   
   if(a_length>b_length)
   {
      out[0] = a[0];
      out[1] = a[1];
   }
   else
   {
      out[0] = b[0];
      out[1] = b[1];
   }
}

void ULK_vector_2d_min(ULK_vector_2d out, const ULK_vector_2d a, const ULK_vector_2d b)
{
   float a_length = a[0]*a[0]+a[1]*a[1];
   float b_length = b[0]*b[0]+b[1]*b[1];
   
   if(a_length<b_length)
   {
      out[0] = a[0];
      out[1] = a[1];
   }
   else
   {
      out[0] = b[0];
      out[1] = b[1];
   }
}

void ULK_vector_2d_multiply(ULK_vector_2d out, const ULK_vector_2d a, const ULK_vector_2d b)
{
   out[0] = a[0]*b[0];
   out[1] = a[1]*b[1];
}

void ULK_vector_2d_negate(ULK_vector_2d out, const ULK_vector_2d in)
{
   out[0] = -in[0];
   out[1] = -in[1];
}

void ULK_vector_2d_normalize(ULK_vector_2d out, const ULK_vector_2d in)
{
   float length = in[0]*in[0]+in[1]*in[1];
   if(length>0)
      length = 1.0f/sqrtf(length);
      
   out[0] = in[0]*length;
   out[1] = in[1]*length;
}

void ULK_vector_2d_rotate(ULK_vector_2d out, const ULK_vector_2d in, const ULK_vector_2d origin, const float radians)
{
   float point0 = in[0]-origin[0];
   float point1 = in[1]-origin[1];
   float sin_rad = sinf(radians);
   float cos_rad = cosf(radians);
   
   out[0] = point0*cos_rad-point1*sin_rad+origin[0];
   out[1] = point0*sin_rad+point1*cos_rad+origin[1];
}

void ULK_vector_2d_round(ULK_vector_2d out, const ULK_vector_2d in)
{
   out[0] = roundf(in[0]);
   out[1] = roundf(in[1]);
}

void ULK_vector_2d_scale(ULK_vector_2d out, const ULK_vector_2d in, const float scale)
{
   out[0] = in[0]*scale;
   out[1] = in[1]*scale;
}

void ULK_vector_2d_set(ULK_vector_2d out, const float x, const float y)
{
   out[0] = x;
   out[1] = y;
}

float ULK_vector_2d_squared_distance(const ULK_vector_2d a, const ULK_vector_2d b)
{
   float x = b[0]-a[0];
   float y = b[1]-a[1];
   
   return x*x+y*y;
}

float ULK_vector_2d_squared_length(const ULK_vector_2d in)
{
   return in[0]*in[0]+in[1]*in[1];
}

void ULK_vector_2d_subtract(ULK_vector_2d out, const ULK_vector_2d a, const ULK_vector_2d b)
{
   out[0] = a[0]-b[0];
   out[1] = a[1]-b[1];
}

void ULK_vector_2d_zero(ULK_vector_2d out)
{
   out[0] = 0.0f;
   out[1] = 0.0f;
}

void ULK_vector_3d_add(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b)
{
   out[0] = a[0]+b[0];
   out[1] = a[1]+b[1];
   out[2] = a[2]+b[2];
}

float ULK_vector_3d_angle(const ULK_vector_3d a, const ULK_vector_3d b)
{
   float magnitude = sqrtf(a[0]*a[0]+a[1]*a[1]+a[2]*a[2])*sqrtf(b[0]*b[0]+b[1]*b[1]+b[2]*b[2]);
   float cosine = 0.0f;
   
   if(magnitude!=0.0f)
      cosine = ULK_vector_3d_dot(a,b)/magnitude;
      
   return acosf(cosine);
}

void ULK_vector_3d_bezier(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b, const ULK_vector_3d c, const ULK_vector_3d d, const float t)
{
   float factor0 = (1-t)*(1-t)*(1-t);
   float factor1 = 3*t*(1-t)*(1-t);
   float factor2 = 3*t*t*(1-t);
   float factor3 = t*t*t;
   
   out[0] = a[0]*factor0+b[0]*factor1+c[0]*factor2+d[0]*factor3;
   out[1] = a[1]*factor0+b[1]*factor1+c[1]*factor2+d[1]*factor3;
   out[2] = a[2]*factor0+b[2]*factor1+c[2]*factor2+d[2]*factor3;
}

void ULK_vector_3d_ceil(ULK_vector_3d out, const ULK_vector_3d in)
{
   out[0] = ceilf(in[0]);
   out[1] = ceilf(in[1]);
   out[2] = ceilf(in[2]);
}

ULK_vector_3d *ULK_vector_3d_clone(const ULK_vector_3d in)
{
   ULK_vector_3d *out = malloc(sizeof(ULK_vector_3d));
   
   *out[0] = in[0];
   *out[1] = in[1];
   *out[2] = in[2];
   
   return out;
}

void ULK_vector_3d_copy(ULK_vector_3d dst, const ULK_vector_3d src)
{
   dst[0] = src[0];
   dst[1] = src[1];
   dst[2] = src[2];
}

ULK_vector_3d *ULK_vector_3d_create()
{
   ULK_vector_3d *out = malloc(sizeof(ULK_vector_3d));
   
   *out[0] = 0.0f;
   *out[1] = 0.0f;
   *out[2] = 0.0f;
   
   return out;
}

void ULK_vector_3d_cross(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b)
{
   float a0 = a[0];
   float a1 = a[1];
   float a2 = a[2];
   float b0 = b[0];
   float b1 = b[1];
   float b2 = b[2];
      
   out[0] = a1*b2-a2*b1;
   out[1] = a2*b0-a0*b2;
   out[2] = a0*b1-a1*b0;
}

float ULK_vector_3d_distance(const ULK_vector_3d a, const ULK_vector_3d b)
{
   float x = b[0]-a[0];
   float y = b[1]-a[1];
   float z = b[2]-a[2];
   
   return sqrtf(x*x+y*y+z*z);
}

void ULK_vector_3d_divide(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b)
{
   out[0] = a[0]/b[0];
   out[1] = a[1]/b[1];
   out[2] = a[2]/b[2];
}

float ULK_vector_3d_dot(const ULK_vector_3d a, const ULK_vector_3d b)
{
   return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}

int ULK_vector_3d_equals(const ULK_vector_3d a, const ULK_vector_3d b)
{
   return (fabs(a[0]-b[0])<=EPSILON*MAX(1.0f,MAX(fabs(a[0]),fabs(b[0])))&&fabs(a[1]-b[1])<=EPSILON*MAX(1.0f,MAX(fabs(a[1]),fabs(b[1])))&&fabs(a[2]-b[2])<=EPSILON*MAX(1.0f,MAX(fabs(a[2]),fabs(b[2]))));
}

int ULK_vector_3d_exact_equals(const ULK_vector_3d a, const ULK_vector_3d b)
{
   return (a[0]==b[0]&&a[1]==b[1]&&a[2]==b[2]);
}

void ULK_vector_3d_floor(ULK_vector_3d out, const ULK_vector_3d in)
{
   out[0] = floorf(in[0]);
   out[1] = floorf(in[1]);
   out[2] = floorf(in[2]);
}

ULK_vector_3d *ULK_vector_3d_from_values(const float x, const float y, const float z)
{
   ULK_vector_3d *out = malloc(sizeof(ULK_vector_3d));
   
   *out[0] = x;
   *out[1] = y;
   *out[2] = z;
   
   return out;
}

void ULK_vector_3d_hermite(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b, const ULK_vector_3d c, const ULK_vector_3d d, float t)
{
   float factor0 = t*t*(2*t-3)+1;
   float factor1 = t*t*(t-2)+t;
   float factor2 = t*t*(t-1);
   float factor3 = t*t*(3-2*t);
   
   out[0] = a[0]*factor0+b[0]*factor1+c[0]*factor2+d[0]*factor3;
   out[1] = a[1]*factor0+b[1]*factor1+c[1]*factor2+d[1]*factor3;
   out[2] = a[2]*factor0+b[2]*factor1+c[2]*factor2+d[2]*factor3;
}

void ULK_vector_3d_inverse(ULK_vector_3d out, const ULK_vector_3d in)
{
   out[0] = 1.0f/in[0];
   out[1] = 1.0f/in[1];
   out[2] = 1.0f/in[2];
}

float ULK_vector_3d_length(const ULK_vector_3d in)
{
   return sqrtf(in[0]*in[0]+in[1]*in[1]+in[2]*in[2]);
}

void ULK_vector_3d_lerp(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b, const float t)
{
   out[0] = a[0]+t*(b[0]-a[0]);
   out[1] = a[1]+t*(b[1]-a[1]);
   out[2] = a[2]+t*(b[2]-a[2]);
}

void ULK_vector_3d_max(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b)
{
   float a_length = a[0]*a[0]+a[1]*a[1]+a[2]*a[2];
   float b_length = b[0]*b[0]+b[1]*b[1]+b[2]*b[2];
   
   if(a_length>b_length)
   {
      out[0] = a[0];
      out[1] = a[1];
      out[2] = a[2];
   }
   else
   {
      out[0] = b[0];
      out[1] = b[1];
      out[2] = b[2];
   }
}

void ULK_vector_3d_min(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b)
{
   float a_length = a[0]*a[0]+a[1]*a[1]+a[2]*a[2];
   float b_length = b[0]*b[0]+b[1]*b[1]+b[2]*b[2];
   
   if(a_length<b_length)
   {
      out[0] = a[0];
      out[1] = a[1];
      out[2] = a[2];
   }
   else
   {
      out[0] = b[0];
      out[1] = b[1];
      out[2] = b[2];
   }
}

void ULK_vector_3d_multiply(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b)
{
   out[0] = a[0]*b[0];
   out[1] = a[1]*b[1];
   out[2] = a[2]*b[2];
}

void ULK_vector_3d_negate(ULK_vector_3d out, const ULK_vector_3d in)
{
   out[0] = -in[0];
   out[1] = -in[1];
   out[2] = -in[2];
}

void ULK_vector_3d_normalize(ULK_vector_3d out, const ULK_vector_3d in)
{
   float length = in[0]*in[0]+in[1]*in[1]+in[2]*in[2];
   
   if(length>0)
      length = 1.0f/sqrtf(length);
      
   out[0] = in[0]*length;
   out[1] = in[1]*length;
   out[2] = in[2]*length;
}

void ULK_vector_3d_rotate_x(ULK_vector_3d out, const ULK_vector_3d in, const ULK_vector_3d origin, const float rad)
{
   ULK_vector_3d p;
   p[0] = in[0]-origin[0];
   p[1] = in[1]-origin[1];
   p[2] = in[2]-origin[2];
   
   out[0] = p[0]+origin[0];
   out[1] = p[1]*cosf(rad)-p[2]*sinf(rad)+origin[1];
   out[2] = p[1]*sinf(rad)+p[2]*cosf(rad)+origin[2];
}

void ULK_vector_3d_rotate_y(ULK_vector_3d out, const ULK_vector_3d in, const ULK_vector_3d origin, const float rad)
{
   ULK_vector_3d p;
   p[0] = in[0]-origin[0];
   p[1] = in[1]-origin[1];
   p[2] = in[2]-origin[2];
   
   out[0] = p[2]*sinf(rad)+p[0]*cosf(rad)+origin[0];
   out[1] = p[1]+origin[1];
   out[2] = p[2]*cosf(rad)-p[0]*sinf(rad)+origin[2];
}

void ULK_vector_3d_rotate_z(ULK_vector_3d out, const ULK_vector_3d in, const ULK_vector_3d origin, const float rad)
{
   ULK_vector_3d p;
   p[0] = in[0]-origin[0];
   p[1] = in[1]-origin[1];
   p[2] = in[2]-origin[2];
   
   out[0] = p[0]*cosf(rad)-p[1]*sinf(rad)+origin[0];
   out[1] = p[0]*sinf(rad)+p[1]*cosf(rad)+origin[1];
   out[2] = p[2]+origin[2];
}

void ULK_vector_3d_round(ULK_vector_3d out, const ULK_vector_3d in)
{
   out[0] = roundf(in[0]);
   out[1] = roundf(in[1]);
   out[2] = roundf(in[2]);
}

void ULK_vector_3d_scale(ULK_vector_3d out, const ULK_vector_3d in, const float scale)
{
   out[0] = in[0]*scale;
   out[1] = in[1]*scale;
   out[2] = in[2]*scale;
}

void ULK_vector_3d_set(ULK_vector_3d out, const float x, const float y, const float z)
{
   out[0] = x;
   out[1] = y;
   out[2] = z;
}

float ULK_vector_3d_squared_distance(const ULK_vector_3d a, const ULK_vector_3d b)
{
   float x = b[0]-a[0];
   float y = b[1]-a[1];
   float z = b[2]-a[2];
   
   return x*x+y*y+z*z;
}

float ULK_vector_3d_squared_length(const ULK_vector_3d in)
{
   return in[0]*in[0]+in[1]*in[1]+in[2]*in[2];
}

void ULK_vector_3d_subtract(ULK_vector_3d out, const ULK_vector_3d a, const ULK_vector_3d b)
{
   out[0] = a[0]-b[0];
   out[1] = a[1]-b[1];
   out[2] = a[2]-b[2];
}

void ULK_vector_3d_zero(ULK_vector_3d out)
{
   out[0] = 0.0f;
   out[1] = 0.0f;
   out[2] = 0.0f;
}

void ULK_vector_4d_add(ULK_vector_4d out, const ULK_vector_4d a, const ULK_vector_4d b)
{
   out[0] = a[0]+b[0];
   out[1] = a[1]+b[1];
   out[2] = a[2]+b[2];
   out[3] = a[3]+b[3];
}

void ULK_vector_4d_ceil(ULK_vector_4d out, const ULK_vector_4d in)
{
   out[0] = ceilf(in[0]);
   out[1] = ceilf(in[1]);
   out[2] = ceilf(in[2]);
   out[3] = ceilf(in[3]);
}

ULK_vector_4d *ULK_vector_4d_clone(const ULK_vector_4d in)
{
   ULK_vector_4d *out = malloc(sizeof(ULK_vector_4d));
   
   *out[0] = in[0];
   *out[1] = in[1];
   *out[2] = in[2];
   *out[3] = in[3];
   
   return out;
}

void ULK_vector_4d_copy(ULK_vector_4d out, const ULK_vector_4d in)
{
   out[0] = in[0];
   out[1] = in[1];
   out[2] = in[2];
   out[3] = in[3];
}

ULK_vector_4d *ULK_vector_4d_create()
{
   ULK_vector_4d *out = malloc(sizeof(ULK_vector_4d));
   
   *out[0] = 0.0f;
   *out[1] = 0.0f;
   *out[2] = 0.0f;
   *out[3] = 0.0f;
   
   return out;
}

void ULK_vector_4d_cross(ULK_vector_4d out, const ULK_vector_4d a, const ULK_vector_4d b, const ULK_vector_4d c)
{
   float A = b[0]*c[1]-b[1]*c[0];
   float B = b[0]*c[2]-b[2]*c[0];
   float C = b[0]*c[3]-b[3]*c[0];
   float D = b[1]*c[2]-b[2]*c[1];
   float E = b[1]*c[3]-b[3]*c[1];
   float F = b[2]*c[3]-b[3]*c[2];
   float G = a[0];
   float H = a[1];
   float I = a[2];
   float J = a[3];
   
   out[0] = H*F-I*E+J*D;
   out[1] = -(G*F)+I*C-J*B;
   out[2] = G*E-H*C+J*A;
   out[3] = -(G*D)+H*B-I*A;
}

float ULK_vector_4d_distance(const ULK_vector_4d a, const ULK_vector_4d b)
{
   float x = b[0]-a[0];
   float y = b[1]-a[1];
   float z = b[2]-a[2];
   float w = b[3]-a[3];
   
   return sqrtf(x*x+y*y+z*z+w*w);
}

void ULK_vector_4d_divide(ULK_vector_4d out, const ULK_vector_4d a, const ULK_vector_4d b)
{
   out[0] = a[0]/b[0];
   out[1] = a[1]/b[1];
   out[2] = a[2]/b[2];
   out[3] = a[3]/b[3];
}

float ULK_vector_4d_dot(const ULK_vector_4d a, const ULK_vector_4d b)
{
   return a[0]*b[0]+a[1]*b[1]+a[2]*b[2]+a[3]*b[3];
}

int ULK_vector_4d_equals(const ULK_vector_4d a, const ULK_vector_4d b)
{
   return (fabs(a[0]-b[0])<=EPSILON*MAX(1.0f,MAX(fabs(a[0]),fabs(b[0])))&&fabs(a[1]-b[1])<=EPSILON*MAX(1.0f,MAX(fabs(a[1]),fabs(b[1])))&&fabs(a[2]-b[2])<=EPSILON*MAX(1.0f,MAX(fabs(a[2]),fabs(b[2])))&&fabs(a[3]-b[3])<=EPSILON*MAX(1.0f,MAX(fabs(a[3]),fabs(b[3]))));
}

int ULK_vector_4d_exact_equals(const ULK_vector_4d a, const ULK_vector_4d b)
{
   return (a[0]==b[0]&&a[1]==b[1]&&a[2]==b[2]&&a[3]==b[3]);
}

void ULK_vector_4d_floor(ULK_vector_4d out, const ULK_vector_4d in)
{
   out[0] = floorf(in[0]);
   out[1] = floorf(in[1]);
   out[2] = floorf(in[2]);
   out[3] = floorf(in[3]);
}

ULK_vector_4d *ULK_vector_4d_from_values(const float x, const float y, const float z, const float w)
{
   ULK_vector_4d *out = malloc(sizeof(ULK_vector_4d));
   
   *out[0] = x;
   *out[1] = y;
   *out[2] = z;
   *out[3] = w;
   
   return out;
}

void ULK_vector_4d_inverse(ULK_vector_4d out, const ULK_vector_4d in)
{
   out[0] = 1.0f/in[0];
   out[1] = 1.0f/in[1];
   out[2] = 1.0f/in[2];
   out[3] = 1.0f/in[3];
}

float ULK_vector_4d_length(const ULK_vector_4d in)
{
   return sqrtf(in[0]*in[0]+in[1]*in[1]+in[2]*in[2]+in[3]*in[3]);
}

void ULK_vector_4d_lerp(ULK_vector_4d out, const ULK_vector_4d a, const ULK_vector_4d b, const float t)
{
   out[0] = a[0]+t*(b[0]-a[0]);
   out[1] = a[1]+t*(b[1]-a[1]);
   out[2] = a[2]+t*(b[2]-a[2]);
   out[3] = a[3]+t*(b[3]-a[3]);
}

void ULK_vector_4d_max(ULK_vector_4d out, const ULK_vector_4d a, const ULK_vector_4d b)
{
   float a_length = a[0]*a[0]+a[1]*a[1]+a[2]*a[2]+a[3]*a[3];
   float b_length = b[0]*b[0]+b[1]*b[1]+b[2]*b[2]+b[3]*b[3];
   
   if(a_length>b_length)
   {
      out[0] = a[0];
      out[1] = a[1];
      out[2] = a[2];
      out[3] = a[3];
   }
   else
   {
      out[0] = b[0];
      out[1] = b[1];
      out[2] = b[2];
      out[3] = b[3];
   }
}

void ULK_vector_4d_min(ULK_vector_4d out, const ULK_vector_4d a, const ULK_vector_4d b)
{
   float a_length = a[0]*a[0]+a[1]*a[1]+a[2]*a[2]+a[3]*a[3];
   float b_length = b[0]*b[0]+b[1]*b[1]+b[2]*b[2]+b[3]*b[3];
   
   if(a_length<b_length)
   {
      out[0] = a[0];
      out[1] = a[1];
      out[2] = a[2];
      out[3] = a[3];
   }
   else
   {
      out[0] = b[0];
      out[1] = b[1];
      out[2] = b[2];
      out[3] = b[3];
   }
}

void ULK_vector_4d_multiply(ULK_vector_4d out, const ULK_vector_4d a, const ULK_vector_4d b)
{
   out[0] = a[0]*b[0];
   out[1] = a[1]*b[1];
   out[2] = a[2]*b[2];
   out[3] = a[3]*b[3];
}

void ULK_vector_4d_negate(ULK_vector_4d out, const ULK_vector_4d in)
{
   out[0] = -in[0];
   out[1] = -in[1];
   out[2] = -in[2];
   out[3] = -in[3];
}

void ULK_vector_4d_normalize(ULK_vector_4d out, const ULK_vector_4d in)
{
   float length = in[0]*in[0]+in[1]*in[1]+in[2]*in[2]+in[3]*in[3];
   
   if(length>0)
      length = 1.0f/sqrtf(length);
      
   out[0] = in[0]*length;
   out[1] = in[1]*length;
   out[2] = in[2]*length;
   out[3] = in[3]*length;
}

void ULK_vector_4d_round(ULK_vector_4d out, const ULK_vector_4d in)
{
   out[0] = roundf(in[0]);
   out[1] = roundf(in[1]);
   out[2] = roundf(in[2]);
   out[3] = roundf(in[3]);
}

void ULK_vector_4d_scale(ULK_vector_4d out, const ULK_vector_4d in, const float scale)
{
   out[0] = in[0]*scale;
   out[1] = in[1]*scale;
   out[2] = in[2]*scale;
   out[3] = in[3]*scale;
}

void ULK_vector_4d_set(ULK_vector_4d out, const float x, const float y, const float z, const float w)
{
   out[0] = x;
   out[1] = y;
   out[2] = z;
   out[3] = w;
}

float ULK_vector_4d_squared_distance(const ULK_vector_4d a, const ULK_vector_4d b)
{
   float x = b[0]-a[0];
   float y = b[1]-a[1];
   float z = b[2]-a[2];
   float w = b[3]-a[3];
   
   return x*x+y*y+z*z+w*w;
}

float ULK_vector_4d_squared_length(const ULK_vector_4d in)
{
   return in[0]*in[0]+in[1]*in[1]+in[2]*in[2]+in[3]*in[3];
}

void ULK_vector_4d_subtract(ULK_vector_4d out, const ULK_vector_4d a, const ULK_vector_4d b)
{
   out[0] = a[0]-b[0];
   out[1] = a[1]-b[1];
   out[2] = a[2]-b[2];
   out[3] = a[3]-b[3];
}

void ULK_vector_4d_zero(ULK_vector_4d out)
{
   out[0] = 0.0f;
   out[1] = 0.0f;
   out[2] = 0.0f;
   out[3] = 0.0f;
}
