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

#ifndef _SLK_3D_CONFIG_H_

#define _SLK_3D_CONFIG_H_

//Configuration:

//This software renderer can only render to a fixed size texture, since
//it could improve performance. A non-fixed size renderer will
//also be released (or has already been).
#define X_RES 640
#define Y_RES 480
#define X_RES_HALF 320
#define Y_RES_HALF 240

//Fixed point percision, must be power of 2.
#define POINT_PER 65536   
#define POINT_PER_POWER 16

//How many affine pixels to draw between perspective corrections in subaffine 
//polygon renderer.
#define SUB_SPAN 64

//Wether to use interlacing, nearly doubles rendering speed at a small loss of
//visual quality, DO NOT clear the screen while having this enabled 
//(unless you want to rape your eyes). Only recommended for resolutions 
//higher than 100x100.
#define INTERLACING 1

//Factor to multiply the z values by, this software renderer can only
//render textured polygons with a z value of less than 1 (for performance reasons).
#define Z_FACTOR 1.0f

//Use branchless macros when possible.
#define BRANCHLESS_MACROS 1

//Use backface culling
//0 for no culling
//1 for counter clockwise culling
//2 for clockwise culling
#define BACKFACE_CULLING 1

//Use spawn buffering,
//buffers polygons first, all of them
//get draw after SLK_3d_dispatch() function call
#define USE_SPAN_BUFFER 1

#if USE_SPAN_BUFFER

//Use z sorting of polygons
#define SPAN_ZSORT 1

//Write to the zbuffer when drawing
//spans (write but not read)
#define WRITE_Z_BUFFER 0

#endif

#endif
