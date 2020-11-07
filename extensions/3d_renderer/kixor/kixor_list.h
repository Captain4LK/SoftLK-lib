#ifndef __LIST_H
#define __LIST_H

typedef struct
{
	int item_count;
	int current_max_size;
	char growable;

	void **items;
	char **names;	
}Kixor_list;

void kixor_list_make(Kixor_list *listo, int size, char growable);
int kixor_list_add_item(Kixor_list *listo, void *item, char *name);
char* kixor_list_print_items(Kixor_list *listo);
void* kixor_list_get_name(Kixor_list *listo, char *name);
void* kixor_list_get_index(Kixor_list *listo, int indx);
void* kixor_list_get_item(Kixor_list *listo, void *item_to_find);
int kixor_list_find(Kixor_list *listo, char *name_to_find);
void kixor_list_delete_index(Kixor_list *listo, int indx);
void kixor_list_delete_name(Kixor_list *listo, char *name);
void kixor_list_delete_item(Kixor_list *listo, void *item);
void kixor_list_delete_all(Kixor_list *listo);
void kixor_list_print_list(Kixor_list *listo);
void kixor_list_free(Kixor_list *listo);

void kixor_test_list();

#endif
