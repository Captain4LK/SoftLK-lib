//External includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//-------------------------------------

//Internal includes
#include "kixor_obj_parser.h"
#include "kixor_list.h"
#include "kixor_string_extra.h"
//-------------------------------------

//#defines
#define WHITESPACE " \t\n\r"
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
//-------------------------------------

//Function prototypes
//-------------------------------------

//Function implementations

void kixor_obj_free_half_list(Kixor_list *listo)
{
   kixor_list_delete_all(listo);
   free(listo->names);
}

int kixor_obj_convert_to_list_index(int current_max, int index)
{
   if(index == 0)  //no index
      return -1;
      
   if(index < 0)  //relative to current list position
      return current_max + index;
      
   return index - 1;  //normal counting index
}

void kixor_obj_convert_to_list_index_v(int current_max, int *indices)
{
   for(int i=0; i<MAX_VERTEX_COUNT; i++)
      indices[i] = kixor_obj_convert_to_list_index(current_max, indices[i]);
}

void kixor_obj_set_material_defaults(Kixor_obj_material *mtl)
{
   mtl->amb[0] = 0.2;
   mtl->amb[1] = 0.2;
   mtl->amb[2] = 0.2;
   mtl->diff[0] = 0.8;
   mtl->diff[1] = 0.8;
   mtl->diff[2] = 0.8;
   mtl->spec[0] = 1.0;
   mtl->spec[1] = 1.0;
   mtl->spec[2] = 1.0;
   mtl->reflect = 0.0;
   mtl->trans = 1;
   mtl->glossy = 98;
   mtl->shiny = 0;
   mtl->refract_index = 1;
   mtl->texture_filename[0] = '\0';
}

int kixor_obj_parse_vertex_index(int *vertex_index, int *texture_index, int *normal_index)
{
   char *temp_str;
   char *token;
   int vertex_count = 0;

   
   while( (token = strtok(NULL, WHITESPACE)) != NULL)
   {
      if(texture_index != NULL)
         texture_index[vertex_count] = 0;
      if(normal_index != NULL)
      normal_index[vertex_count] = 0;

      vertex_index[vertex_count] = atoi( token );
      
      if(kixor_contains(token, "//"))  //normal only
      {
         temp_str = strchr(token, '/');
         temp_str++;
         normal_index[vertex_count] = atoi( ++temp_str );
      }
      else if(kixor_contains(token, "/"))
      {
         temp_str = strchr(token, '/');
         texture_index[vertex_count] = atoi( ++temp_str );

         if(kixor_contains(temp_str, "/"))
         {
            temp_str = strchr(temp_str, '/');
            normal_index[vertex_count] = atoi( ++temp_str );
         }
      }
      
      vertex_count++;
   }

   return vertex_count;
}

Kixor_obj_face* kixor_obj_parse_face(Kixor_obj_growable_scene_data *scene)
{
   int vertex_count;
   Kixor_obj_face *face = (Kixor_obj_face*)malloc(sizeof(Kixor_obj_face));
   
   vertex_count = kixor_obj_parse_vertex_index(face->vertex_index, face->texture_index, face->normal_index);
   kixor_obj_convert_to_list_index_v(scene->vertex_list.item_count, face->vertex_index);
   kixor_obj_convert_to_list_index_v(scene->vertex_texture_list.item_count, face->texture_index);
   kixor_obj_convert_to_list_index_v(scene->vertex_normal_list.item_count, face->normal_index);
   face->vertex_count = vertex_count;

   return face;
}

Kixor_obj_sphere* kixor_obj_parse_sphere(Kixor_obj_growable_scene_data *scene)
{
   int temp_indices[MAX_VERTEX_COUNT];

   Kixor_obj_sphere *obj = (Kixor_obj_sphere*)malloc(sizeof(Kixor_obj_sphere));
   kixor_obj_parse_vertex_index(temp_indices, obj->texture_index, NULL);
   kixor_obj_convert_to_list_index_v(scene->vertex_texture_list.item_count, obj->texture_index);
   obj->pos_index = kixor_obj_convert_to_list_index(scene->vertex_list.item_count, temp_indices[0]);
   obj->up_normal_index = kixor_obj_convert_to_list_index(scene->vertex_normal_list.item_count, temp_indices[1]);
   obj->equator_normal_index = kixor_obj_convert_to_list_index(scene->vertex_normal_list.item_count, temp_indices[2]);

   return obj;
}

Kixor_obj_plane* kixor_obj_parse_plane(Kixor_obj_growable_scene_data *scene)
{
   int temp_indices[MAX_VERTEX_COUNT];

   Kixor_obj_plane *obj = (Kixor_obj_plane*)malloc(sizeof(Kixor_obj_plane));
   kixor_obj_parse_vertex_index(temp_indices, obj->texture_index, NULL);
   kixor_obj_convert_to_list_index_v(scene->vertex_texture_list.item_count, obj->texture_index);
   obj->pos_index = kixor_obj_convert_to_list_index(scene->vertex_list.item_count, temp_indices[0]);
   obj->normal_index = kixor_obj_convert_to_list_index(scene->vertex_normal_list.item_count, temp_indices[1]);
   obj->rotation_normal_index = kixor_obj_convert_to_list_index(scene->vertex_normal_list.item_count, temp_indices[2]);

   return obj;
}

Kixor_obj_light_point* kixor_obj_parse_light_point(Kixor_obj_growable_scene_data *scene)
{
   Kixor_obj_light_point *o= (Kixor_obj_light_point*)malloc(sizeof(Kixor_obj_light_point));
   o->pos_index = kixor_obj_convert_to_list_index(scene->vertex_list.item_count, atoi( strtok(NULL, WHITESPACE)) );
   return o;
}

Kixor_obj_light_quad* kixor_obj_parse_light_quad(Kixor_obj_growable_scene_data *scene)
{
   Kixor_obj_light_quad *o = (Kixor_obj_light_quad*)malloc(sizeof(Kixor_obj_light_quad));
   kixor_obj_parse_vertex_index(o->vertex_index, NULL, NULL);
   kixor_obj_convert_to_list_index_v(scene->vertex_list.item_count, o->vertex_index);

   return o;
}

Kixor_obj_light_disc* kixor_obj_parse_light_disc(Kixor_obj_growable_scene_data *scene)
{
   int temp_indices[MAX_VERTEX_COUNT];

   Kixor_obj_light_disc *obj = (Kixor_obj_light_disc*)malloc(sizeof(Kixor_obj_light_disc));
   kixor_obj_parse_vertex_index(temp_indices, NULL, NULL);
   obj->pos_index = kixor_obj_convert_to_list_index(scene->vertex_list.item_count, temp_indices[0]);
   obj->normal_index = kixor_obj_convert_to_list_index(scene->vertex_normal_list.item_count, temp_indices[1]);

   return obj;
}

Kixor_obj_vector* kixor_obj_parse_vector()
{
   Kixor_obj_vector *v = (Kixor_obj_vector*)malloc(sizeof(Kixor_obj_vector));

   for(int i = 0;i<3;i++)
   {
      char *tmp = strtok(NULL,WHITESPACE);
      if(tmp!=NULL)
         v->e[i] = atof(tmp);
   }

   return v;
}

void kixor_obj_parse_camera(Kixor_obj_growable_scene_data *scene, Kixor_obj_camera *camera)
{
   int indices[3];
   kixor_obj_parse_vertex_index(indices, NULL, NULL);
   camera->camera_pos_index = kixor_obj_convert_to_list_index(scene->vertex_list.item_count, indices[0]);
   camera->camera_look_point_index = kixor_obj_convert_to_list_index(scene->vertex_list.item_count, indices[1]);
   camera->camera_up_norm_index = kixor_obj_convert_to_list_index(scene->vertex_normal_list.item_count, indices[2]);
}

int kixor_obj_parse_mtl_file(char *filename, Kixor_list *material_list)
{
   int line_number = 0;
   char *current_token;
   char current_line[OBJ_LINE_SIZE];
   char material_open = 0;
   Kixor_obj_material *current_mtl = NULL;
   FILE *mtl_file_stream;
   
   // open scene
   mtl_file_stream = fopen( filename, "r");
   if(mtl_file_stream == 0)
   {
      fprintf(stderr, "Error reading file: %s\n", filename);
      return 0;
   }
      
   kixor_list_make(material_list, 10, 1);

   while( fgets(current_line, OBJ_LINE_SIZE, mtl_file_stream) )
   {
      current_token = strtok( current_line, " \t\n\r");
      line_number++;
      
      //skip comments
      if( current_token == NULL || kixor_strequal(current_token, "//") || kixor_strequal(current_token, "#"))
         continue;
      

      //start material
      else if( kixor_strequal(current_token, "newmtl"))
      {
         material_open = 1;
         current_mtl = (Kixor_obj_material*) malloc(sizeof(Kixor_obj_material));
         kixor_obj_set_material_defaults(current_mtl);
         
         // get the name
         strncpy(current_mtl->name, strtok(NULL, " \t"), MATERIAL_NAME_SIZE);
         kixor_list_add_item(material_list, current_mtl, current_mtl->name);
      }
      
      //ambient
      else if( kixor_strequal(current_token, "Ka") && material_open)
      {
         current_mtl->amb[0] = atof( strtok(NULL, " \t"));
         current_mtl->amb[1] = atof( strtok(NULL, " \t"));
         current_mtl->amb[2] = atof( strtok(NULL, " \t"));
      }

      //diff
      else if( kixor_strequal(current_token, "Kd") && material_open)
      {
         current_mtl->diff[0] = atof( strtok(NULL, " \t"));
         current_mtl->diff[1] = atof( strtok(NULL, " \t"));
         current_mtl->diff[2] = atof( strtok(NULL, " \t"));
      }
      
      //specular
      else if( kixor_strequal(current_token, "Ks") && material_open)
      {
         current_mtl->spec[0] = atof( strtok(NULL, " \t"));
         current_mtl->spec[1] = atof( strtok(NULL, " \t"));
         current_mtl->spec[2] = atof( strtok(NULL, " \t"));
      }
      //shiny
      else if( kixor_strequal(current_token, "Ns") && material_open)
      {
         current_mtl->shiny = atof( strtok(NULL, " \t"));
      }
      //transparent
      else if( kixor_strequal(current_token, "d") && material_open)
      {
         current_mtl->trans = atof( strtok(NULL, " \t"));
      }
      //reflection
      else if( kixor_strequal(current_token, "r") && material_open)
      {
         current_mtl->reflect = atof( strtok(NULL, " \t"));
      }
      //glossy
      else if( kixor_strequal(current_token, "sharpness") && material_open)
      {
         current_mtl->glossy = atof( strtok(NULL, " \t"));
      }
      //refract index
      else if( kixor_strequal(current_token, "Ni") && material_open)
      {
         current_mtl->refract_index = atof( strtok(NULL, " \t"));
      }
      // illumination type
      else if( kixor_strequal(current_token, "illum") && material_open)
      {
      }
      // emissive coefficient
      else if( kixor_strequal(current_token, "Ke") && material_open)
      {
      }
      // texture map
      else if( kixor_strequal(current_token, "map_Kd") && material_open)
      {
         strncpy(current_mtl->texture_filename, strtok(NULL, WHITESPACE), OBJ_FILENAME_LENGTH);
         current_mtl->texture_filename[strlen(current_mtl->texture_filename)] = 0;
      }
      else
      {
         fprintf(stderr, "Unknown command '%s' in material file %s at line %i:\n\t%s\n",
               current_token, filename, line_number, current_line);
         //return 0;
      }
   }
   
   fclose(mtl_file_stream);

   return 1;
}

int kixor_obj_parse_obj_file(Kixor_obj_growable_scene_data *growable_data, char *filename)
{
   FILE* obj_file_stream;
   int current_material = -1; 
   char *current_token = NULL;
   char current_line[OBJ_LINE_SIZE];
   int line_number = 0;
   // open scene
   obj_file_stream = fopen( filename, "r");
   if(obj_file_stream == 0)
   {
      fprintf(stderr, "Error reading file: %s\n", filename);
      return 0;
   }

/*    
   extreme_dimensions[0].x = INFINITY; extreme_dimensions[0].y = INFINITY; extreme_dimensions[0].z = INFINITY;
   extreme_dimensions[1].x = -INFINITY; extreme_dimensions[1].y = -INFINITY; extreme_dimensions[1].z = -INFINITY;

         if(v->x < extreme_dimensions[0].x) extreme_dimensions[0].x = v->x;
         if(v->x > extreme_dimensions[1].x) extreme_dimensions[1].x = v->x;
         if(v->y < extreme_dimensions[0].y) extreme_dimensions[0].y = v->y;
         if(v->y > extreme_dimensions[1].y) extreme_dimensions[1].y = v->y;
         if(v->z < extreme_dimensions[0].z) extreme_dimensions[0].z = v->z;
         if(v->z > extreme_dimensions[1].z) extreme_dimensions[1].z = v->z;*/


   //parser loop
   while( fgets(current_line, OBJ_LINE_SIZE, obj_file_stream) )
   {
      current_token = strtok( current_line, " \t\n\r");
      line_number++;
      
      //skip comments
      if( current_token == NULL || current_token[0] == '#')
         continue;

      //parse objects
      else if( kixor_strequal(current_token, "v") ) //process vertex
      {
         kixor_list_add_item(&growable_data->vertex_list,  kixor_obj_parse_vector(), NULL);
      }
      
      else if( kixor_strequal(current_token, "vn") ) //process vertex normal
      {
         kixor_list_add_item(&growable_data->vertex_normal_list,  kixor_obj_parse_vector(), NULL);
      }
      
      else if( kixor_strequal(current_token, "vt") ) //process vertex texture
      {
         kixor_list_add_item(&growable_data->vertex_texture_list,  kixor_obj_parse_vector(), NULL);
      }
      
      else if( kixor_strequal(current_token, "f") ) //process face
      {
         Kixor_obj_face *face = kixor_obj_parse_face(growable_data);
         face->material_index = current_material;
         kixor_list_add_item(&growable_data->face_list, face, NULL);
      }
      
      else if( kixor_strequal(current_token, "sp") ) //process sphere
      {
         Kixor_obj_sphere *sphr = kixor_obj_parse_sphere(growable_data);
         sphr->material_index = current_material;
         kixor_list_add_item(&growable_data->sphere_list, sphr, NULL);
      }
      
      else if( kixor_strequal(current_token, "pl") ) //process plane
      {
         Kixor_obj_plane *pl = kixor_obj_parse_plane(growable_data);
         pl->material_index = current_material;
         kixor_list_add_item(&growable_data->plane_list, pl, NULL);
      }
      
      else if( kixor_strequal(current_token, "p") ) //process point
      {
         //make a small sphere to represent the point?
      }
      
      else if( kixor_strequal(current_token, "lp") ) //light point source
      {
         Kixor_obj_light_point *o = kixor_obj_parse_light_point(growable_data);
         o->material_index = current_material;
         kixor_list_add_item(&growable_data->light_point_list, o, NULL);
      }
      
      else if( kixor_strequal(current_token, "ld") ) //process light disc
      {
         Kixor_obj_light_disc *o = kixor_obj_parse_light_disc(growable_data);
         o->material_index = current_material;
         kixor_list_add_item(&growable_data->light_disc_list, o, NULL);
      }
      
      else if( kixor_strequal(current_token, "lq") ) //process light quad
      {
         Kixor_obj_light_quad *o = kixor_obj_parse_light_quad(growable_data);
         o->material_index = current_material;
         kixor_list_add_item(&growable_data->light_quad_list, o, NULL);
      }
      
      else if( kixor_strequal(current_token, "c") ) //camera
      {
         growable_data->camera = (Kixor_obj_camera*) malloc(sizeof(Kixor_obj_camera));
         kixor_obj_parse_camera(growable_data, growable_data->camera);
      }
      
      else if( kixor_strequal(current_token, "usemtl") ) // usemtl
      {
         current_material = kixor_list_find(&growable_data->material_list, strtok(NULL, WHITESPACE));
      }
      
      else if( kixor_strequal(current_token, "mtllib") ) // mtllib
      {
         strncpy(growable_data->material_filename, strtok(NULL, WHITESPACE), OBJ_FILENAME_LENGTH);
         kixor_obj_parse_mtl_file(growable_data->material_filename, &growable_data->material_list);
         continue;
      }
      
      else if( kixor_strequal(current_token, "o") ) //object name
      { }
      else if( kixor_strequal(current_token, "s") ) //smoothing
      { }
      else if( kixor_strequal(current_token, "g") ) // group
      { }      
      else if( kixor_strequal(current_token, "l") ) // TODO
      { }      

      else
      {
         printf("Unknown command '%s' in scene code at line %i: \"%s\".\n",
               current_token, line_number, current_line);
      }
   }

   fclose(obj_file_stream);
   
   return 1;
}


void kixor_obj_init_temp_storage(Kixor_obj_growable_scene_data *growable_data)
{
   kixor_list_make(&growable_data->vertex_list, 10, 1);
   kixor_list_make(&growable_data->vertex_normal_list, 10, 1);
   kixor_list_make(&growable_data->vertex_texture_list, 10, 1);
   
   kixor_list_make(&growable_data->face_list, 10, 1);
   kixor_list_make(&growable_data->sphere_list, 10, 1);
   kixor_list_make(&growable_data->plane_list, 10, 1);
   
   kixor_list_make(&growable_data->light_point_list, 10, 1);
   kixor_list_make(&growable_data->light_quad_list, 10, 1);
   kixor_list_make(&growable_data->light_disc_list, 10, 1);
   
   kixor_list_make(&growable_data->material_list, 10, 1);   
   
   growable_data->camera = NULL;
}

void kixor_obj_free_temp_storage(Kixor_obj_growable_scene_data *growable_data)
{
   kixor_obj_free_half_list(&growable_data->vertex_list);
   kixor_obj_free_half_list(&growable_data->vertex_normal_list);
   kixor_obj_free_half_list(&growable_data->vertex_texture_list);
   
   kixor_obj_free_half_list(&growable_data->face_list);
   kixor_obj_free_half_list(&growable_data->sphere_list);
   kixor_obj_free_half_list(&growable_data->plane_list);
   
   kixor_obj_free_half_list(&growable_data->light_point_list);
   kixor_obj_free_half_list(&growable_data->light_quad_list);
   kixor_obj_free_half_list(&growable_data->light_disc_list);
   
   kixor_obj_free_half_list(&growable_data->material_list);
}

void kixor_delete_obj_data(Kixor_obj_scene_data *data_out)
{
   int i;
   
   for(i=0; i<data_out->vertex_count; i++)
      free(data_out->vertex_list[i]);
   free(data_out->vertex_list);
   for(i=0; i<data_out->vertex_normal_count; i++)
      free(data_out->vertex_normal_list[i]);
   free(data_out->vertex_normal_list);
   for(i=0; i<data_out->vertex_texture_count; i++)
      free(data_out->vertex_texture_list[i]);
   free(data_out->vertex_texture_list);

   for(i=0; i<data_out->face_count; i++)
      free(data_out->face_list[i]);
   free(data_out->face_list);
   for(i=0; i<data_out->sphere_count; i++)
      free(data_out->sphere_list[i]);
   free(data_out->sphere_list);
   for(i=0; i<data_out->plane_count; i++)
      free(data_out->plane_list[i]);
   free(data_out->plane_list);

   for(i=0; i<data_out->light_point_count; i++)
      free(data_out->light_point_list[i]);
   free(data_out->light_point_list);
   for(i=0; i<data_out->light_disc_count; i++)
      free(data_out->light_disc_list[i]);
   free(data_out->light_disc_list);
   for(i=0; i<data_out->light_quad_count; i++)
      free(data_out->light_quad_list[i]);
   free(data_out->light_quad_list);

   for(i=0; i<data_out->material_count; i++)
      free(data_out->material_list[i]);
   free(data_out->material_list);

   free(data_out->camera);
}

void kixor_obj_copy_to_out_storage(Kixor_obj_scene_data *data_out, Kixor_obj_growable_scene_data *growable_data)
{
   data_out->vertex_count = growable_data->vertex_list.item_count;
   data_out->vertex_normal_count = growable_data->vertex_normal_list.item_count;
   data_out->vertex_texture_count = growable_data->vertex_texture_list.item_count;

   data_out->face_count = growable_data->face_list.item_count;
   data_out->sphere_count = growable_data->sphere_list.item_count;
   data_out->plane_count = growable_data->plane_list.item_count;

   data_out->light_point_count = growable_data->light_point_list.item_count;
   data_out->light_disc_count = growable_data->light_disc_list.item_count;
   data_out->light_quad_count = growable_data->light_quad_list.item_count;

   data_out->material_count = growable_data->material_list.item_count;
   
   data_out->vertex_list = (Kixor_obj_vector**)growable_data->vertex_list.items;
   data_out->vertex_normal_list = (Kixor_obj_vector**)growable_data->vertex_normal_list.items;
   data_out->vertex_texture_list = (Kixor_obj_vector**)growable_data->vertex_texture_list.items;

   data_out->face_list = (Kixor_obj_face**)growable_data->face_list.items;
   data_out->sphere_list = (Kixor_obj_sphere**)growable_data->sphere_list.items;
   data_out->plane_list = (Kixor_obj_plane**)growable_data->plane_list.items;

   data_out->light_point_list = (Kixor_obj_light_point**)growable_data->light_point_list.items;
   data_out->light_disc_list = (Kixor_obj_light_disc**)growable_data->light_disc_list.items;
   data_out->light_quad_list = (Kixor_obj_light_quad**)growable_data->light_quad_list.items;
   
   data_out->material_list = (Kixor_obj_material**)growable_data->material_list.items;
   
   data_out->camera = growable_data->camera;
}

int kixor_parse_obj_scene(Kixor_obj_scene_data *data_out, char *filename)
{
   Kixor_obj_growable_scene_data growable_data;

   kixor_obj_init_temp_storage(&growable_data);
   if( kixor_obj_parse_obj_file(&growable_data, filename) == 0)
      return 0;
   
   //print_vector(NORMAL, "Max bounds are: ", &growable_data->extreme_dimensions[1]);
   //print_vector(NORMAL, "Min bounds are: ", &growable_data->extreme_dimensions[0]);

   kixor_obj_copy_to_out_storage(data_out, &growable_data);
   kixor_obj_free_temp_storage(&growable_data);
   return 1;
}
//-------------------------------------
