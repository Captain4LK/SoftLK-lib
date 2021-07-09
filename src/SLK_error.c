/*
Copyright (c) 2021, Lukas Holzbeierlein (Captain4LK) 
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//Based on golgotha public domain release.

//External includes
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
//-------------------------------------

//Internal includes
#include "../include/SLK/SLK_config.h"
#include "../include/SLK/SLK_types.h"
#include "../include/SLK/SLK_functions.h"
#include "backend.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
static const char *SLK_error_file_on;
static int SLK_error_line_on;
//-------------------------------------

//Function prototypes
static void SLK_error_old(const char *format, ...);
static void SLK_warning_old(const char *format, ...);
//-------------------------------------

//Function implementations

SLK_error_pointer_type SLK_get_error_function_pointer(const char *file, int line)
{
   SLK_error_file_on=file;
   SLK_error_line_on=line;
   return SLK_error_old;
}

SLK_error_pointer_type SLK_get_warning_function_pointer(const char *file, int line)
{
   SLK_error_file_on=file;
   SLK_error_line_on=line;
   return SLK_warning_old;
}

static void SLK_error_old(const char *format, ...)
{
  va_list ap;
  char st[512];

  va_start(ap, format);
  vsprintf(st, format, ap);
  SLK_default_error(st);
  va_end(ap);
}

static void SLK_warning_old(const char *format, ...)
{
  va_list ap;
  char st[512];

  va_start(ap, format);
  vsprintf(st, format, ap);
  SLK_default_warning(st);
  va_end(ap);
}

void SLK_default_warning(const char *st)
{
   SLK_log("SoftLK warning (%s:%d): %s\n",SLK_error_file_on,SLK_error_line_on,st);
}

void SLK_default_error(const char *st)
{
   SLK_log("SoftLK error (%s:%d): %s\n",SLK_error_file_on,SLK_error_line_on,st);
}

void SLK_log(const char *w, ...)
{
   va_list args;
   va_start(args,w);
   backend_log(w,args);
   va_end(args);
}
//-------------------------------------
