//External includes
#include <string.h>
//-------------------------------------

//Internal includes
#include "kixor_string_extra.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
//-------------------------------------

//Function prototypes
//-------------------------------------

//Function implementations

char kixor_strequal(const char *s1, const char *s2)
{
   if(strcmp(s1, s2) == 0)
      return 1;
   return 0;
}

char kixor_contains(const char *haystack, const char *needle)
{
   if(strstr(haystack, needle) == NULL)
      return 0;
   return 1;
}
//-------------------------------------
