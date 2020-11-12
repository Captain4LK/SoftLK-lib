//External includes
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//-------------------------------------

//Internal includes
#include "kixor_list.h"
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

// internal helper functions
char kixor_list_is_full(Kixor_list *listo)
{
   return(listo->item_count == listo->current_max_size);
}

void kixor_list_grow(Kixor_list *old_listo)
{
   int i;
   Kixor_list new_listo;
   
   kixor_list_make(&new_listo, old_listo->current_max_size*2, old_listo->growable++);
   
   for(i=0; i<old_listo->current_max_size; i++)
      kixor_list_add_item(&new_listo, old_listo->items[i] , old_listo->names[i]);
   
   kixor_list_free(old_listo);
   
   //copy new structure to old list
   old_listo->names = new_listo.names;
   old_listo->items = new_listo.items;
   old_listo->item_count = new_listo.item_count;
   old_listo->current_max_size = new_listo.current_max_size;
   old_listo->growable = new_listo.growable;
}
//end helpers

void kixor_list_make(Kixor_list *listo, int start_size, char growable)
{
   listo->names = (char**) malloc(sizeof(char*) * start_size);
   listo->items = (void**) malloc(sizeof(void*) * start_size);
   listo->item_count = 0;
   listo->current_max_size = start_size;
   listo->growable = growable;
}

int kixor_list_add_item(Kixor_list *listo, void *item, char *name)
{
   int name_length;
   char *new_name;
   
   if( kixor_list_is_full(listo) )
   {
      if( listo->growable )
         kixor_list_grow(listo);
      else
         return -1;
   }
   
   listo->names[listo->item_count] = NULL;
   if(name != NULL)
   {
      name_length = strlen(name);
      new_name = (char*) malloc(sizeof(char) * name_length + 1);
      strcpy(new_name, name);
      listo->names[listo->item_count] = new_name;
   }

   listo->items[listo->item_count] = item;
   listo->item_count++;
   
   return listo->item_count-1;
}

char* kixor_list_print_items(Kixor_list *listo)
{
   int i;

   for(i=0; i < listo->item_count; i++)
   {
      printf("%s\n", listo->names[i]);
   }
   
   return NULL;
}

void* kixor_list_get_index(Kixor_list *listo, int indx)
{
   if(indx < listo->item_count)
      return listo->items[indx];
   return NULL;
}

void* kixor_list_get_item(Kixor_list *listo, void *item_to_find)
{
   int i = 0;
   
   for(i=0; i < listo->item_count; i++)
   {
      if(listo->items[i] == item_to_find)
         return listo->items[i];
   }
   
   return NULL;
}

void* kixor_list_get_name(Kixor_list *listo, char *name_to_find)
{
   int i = 0;

   for(i=0; i < listo->item_count; i++)
   {
      if(strncmp(listo->names[i], name_to_find, strlen(name_to_find)) == 0)
         return listo->items[i];
   }
   
   return NULL;
}

int kixor_list_find(Kixor_list *listo, char *name_to_find)
{
   int i = 0;

   for(i=0; i < listo->item_count; i++)
   {
      if(strncmp(listo->names[i], name_to_find, strlen(name_to_find)) == 0)
         return i;
   }
   
   return -1;
}

void kixor_list_delete_item(Kixor_list *listo, void *item)
{
   int i;
   
   for(i=0; i < listo->item_count; i++)
   {     
      if( listo->items[i] == item )
         kixor_list_delete_index(listo, i);
   }
}

void kixor_list_delete_name(Kixor_list *listo, char *name)
{
   int i;
   //int j;
   //char remove = 0;
   
   // int length_name = strlen(name);
   //int item_name;
   
   if(name == NULL)
      return;
   
   for(i=0; i < listo->item_count; i++)
   {
      //item_name = strlen(name);
            
      if( name != NULL && (strncmp(listo->names[i], name, strlen(name)) == 0) )
         kixor_list_delete_index(listo, i);
   }
}

void kixor_list_delete_index(Kixor_list *listo, int indx)
{
   int j;
   
   //remove item
   if(listo->names[indx] != NULL)
      free(listo->names[indx]);
         
   //restructure
   for(j=indx; j < listo->item_count-1; j++)
   {
      listo->names[j] = listo->names[j+1];
      listo->items[j] = listo->items[j+1];
   }
   
   listo->item_count--;
   
   return;
}

void kixor_list_delete_all(Kixor_list *listo)
{
   int i;
   
   for(i=listo->item_count-1; i>=0; i--)
      kixor_list_delete_index(listo, i);
}

void kixor_list_free(Kixor_list *listo)
{
   kixor_list_delete_all(listo);
   free(listo->names);
   free(listo->items);
}

void kixor_list_print_list(Kixor_list *listo)
{
   int i;
   
   printf("count: %i/%i\n", listo->item_count, listo->current_max_size);
   
   for(i=0; i < listo->item_count; i++)
   {
      printf("list[%i]: %s\n", i, listo->names[i]);
   }
}
//-------------------------------------
