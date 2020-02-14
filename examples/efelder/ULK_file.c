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

#include "ULK_file.h"

char *ULK_file_load(const char *path)
{
   char *data = NULL;
   size_t file_size = 0;
   FILE *f = fopen(path,"rb");

   fseek(f,0,SEEK_END);
   file_size = ftell(f);
   fseek(f,0,SEEK_SET);

   data = malloc(file_size+1);
   fread(data,file_size,1,f);
   data[file_size] = '\0';
   fclose(f);

   return data;
}

void ULK_file_destroy_data(char *data)
{
   if(data!=NULL)
      free(data);
}

void ULK_file_dump(const char *data, const size_t size, const char *path)
{
   FILE *f = fopen(path,"wb");

   fwrite(data,size,1,f);
}
