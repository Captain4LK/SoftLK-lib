#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include "kixor_list.h"

#define OBJ_FILENAME_LENGTH 500
#define MATERIAL_NAME_SIZE 255
#define OBJ_LINE_SIZE 500
#define MAX_VERTEX_COUNT 4 //can only handle quads or triangles

typedef struct Kixor_obj_face
{
	int vertex_index[MAX_VERTEX_COUNT];
	int normal_index[MAX_VERTEX_COUNT];
	int texture_index[MAX_VERTEX_COUNT];
	int vertex_count;
	int material_index;
}Kixor_obj_face;

typedef struct Kixor_obj_sphere
{
	int pos_index;
	int up_normal_index;
	int equator_normal_index;
	int texture_index[MAX_VERTEX_COUNT];
	int material_index;
}Kixor_obj_sphere;

typedef struct Kixor_obj_plane
{
	int pos_index;
	int normal_index;
	int rotation_normal_index;
	int texture_index[MAX_VERTEX_COUNT];
	int material_index;
}Kixor_obj_plane;

typedef struct Kixor_obj_vector
{
	double e[3];
}Kixor_obj_vector;

typedef struct Kixor_obj_material
{
	char name[MATERIAL_NAME_SIZE+1];
	char texture_filename[OBJ_FILENAME_LENGTH+1];
	double amb[3];
	double diff[3];
	double spec[3];
	double reflect;
	double refract;
	double trans;
	double shiny;
	double glossy;
	double refract_index;
}Kixor_obj_material;

typedef struct Kixor_obj_camera
{
	int camera_pos_index;
	int camera_look_point_index;
	int camera_up_norm_index;
}Kixor_obj_camera;

typedef struct Kixor_obj_light_point
{
	int pos_index;
	int material_index;
}Kixor_obj_light_point;

typedef struct Kixor_obj_light_disc
{
	int pos_index;
	int normal_index;
	int material_index;
}Kixor_obj_light_disc;

typedef struct Kixor_obj_light_quad
{
	int vertex_index[MAX_VERTEX_COUNT];
	int material_index;
}Kixor_obj_light_quad;

typedef struct Kixor_obj_growable_scene_data
{
//	vector extreme_dimensions[2];
	char scene_filename[OBJ_FILENAME_LENGTH];
	char material_filename[OBJ_FILENAME_LENGTH+1];
	
	Kixor_list vertex_list;
	Kixor_list vertex_normal_list;
	Kixor_list vertex_texture_list;
	
	Kixor_list face_list;
	Kixor_list sphere_list;
	Kixor_list plane_list;
	
	Kixor_list light_point_list;
	Kixor_list light_quad_list;
	Kixor_list light_disc_list;
	
	Kixor_list material_list;
	
	Kixor_obj_camera *camera;
}Kixor_obj_growable_scene_data;

typedef struct Kixor_obj_scene_data
{
	Kixor_obj_vector **vertex_list;
	Kixor_obj_vector **vertex_normal_list;
	Kixor_obj_vector **vertex_texture_list;
	
	Kixor_obj_face **face_list;
	Kixor_obj_sphere **sphere_list;
	Kixor_obj_plane **plane_list;
	
	Kixor_obj_light_point **light_point_list;
	Kixor_obj_light_quad **light_quad_list;
	Kixor_obj_light_disc **light_disc_list;
	
	Kixor_obj_material **material_list;
	
	int vertex_count;
	int vertex_normal_count;
	int vertex_texture_count;

	int face_count;
	int sphere_count;
	int plane_count;

	int light_point_count;
	int light_quad_count;
	int light_disc_count;

	int material_count;

	Kixor_obj_camera *camera;
}Kixor_obj_scene_data;

int kixor_parse_obj_scene(Kixor_obj_scene_data *data_out, char *filename);
void kixor_delete_obj_data(Kixor_obj_scene_data *data_out);

#endif
