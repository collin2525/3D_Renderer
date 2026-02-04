#define RENDERER_H
typedef struct vector2{double x, y;} vector2;
typedef struct vector3{double x, y, z;} vector3;
typedef struct edge{int start, end;} edge;
typedef struct object3d{
    vector3* vertices;
    edge* edges;
    int vertex_count;
    int edge_count;
    char* name;
} object3d;

extern object3d obj;
extern double fov;

void create_object(double (*vertices)[3], int vertex_count, int (*edges)[2], int edge_count, char* name);
double* get_projected_vertices(int* out_count);
vector2 projection(vector3 vertex);