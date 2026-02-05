#define RENDERER_H
#include "string.h"
typedef struct vector2{double x, y;} vector2;
typedef struct vector3{double x, y, z;} vector3;
typedef struct edge{int start, end;} edge;
typedef struct face{int v1, v2, v3, v4; vector3 color;} face;
typedef struct object3d{
    vector3* vertices;
    edge* edges;
    face* faces;
    int vertex_count;
    int edge_count;
    int face_count;
    String name;
} object3d;

extern object3d objs[10];
extern double fov;

void initialize_renderer(int width, int height, double fov_value);
void create_object(double (*vertices)[3], int vertex_count, int (*edges)[2], int edge_count, int (*faces)[4], int face_count, char* name);
void rotate_cube_x(double angle);
void rotate_cube_y(double angle);
void rotate_cube_z(double angle);

double* get_projected_vertices(int* out_count, char* object_name);
int* get_edges(int* out_count, char* object_name);
double* get_face_projections(int* out_count, char* object_name);
double* get_face_colors(int* out_count, char* object_name);
vector2 projection(vector3 vertex);