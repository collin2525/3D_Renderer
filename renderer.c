#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>

object3d obj;
double fov = 10.0;

void create_object(double (*vertices)[3], int vertex_count, int (*edges)[2], int edge_count, char* name) {
    obj.vertex_count = vertex_count;
    obj.edge_count = edge_count;
    obj.name = name;  // Added assignment for the name field

    obj.vertices = (vector3*)malloc(sizeof(vector3) * vertex_count);
    for (int i = 0; i < vertex_count; i++) {
        obj.vertices[i].x = vertices[i][0];
        obj.vertices[i].y = vertices[i][1];
        obj.vertices[i].z = vertices[i][2];
    }

    obj.edges = (edge*)malloc(sizeof(edge) * edge_count);
    for (int i = 0; i < edge_count; i++) {
        obj.edges[i].start = edges[i][0];
        obj.edges[i].end = edges[i][1];
    }
}

double* get_projected_vertices(int* out_count) {
    *out_count = obj.edge_count * 4; // Each edge contributes 4 values (x1, y1, x2, y2)
    double* projected = (double*)malloc(sizeof(double) * (*out_count));
    int index = 0;

    for (int i = 0; i < obj.edge_count; i++) {
        vector3 v_start = obj.vertices[obj.edges[i].start];
        vector3 v_end = obj.vertices[obj.edges[i].end];

        vector2 p_start = projection(v_start);
        vector2 p_end = projection(v_end);

        projected[index++] = p_start.x;
        projected[index++] = p_start.y;
        projected[index++] = p_end.x;
        projected[index++] = p_end.y;
    }

    return projected;
}

vector2 projection(vector3 vertex) {
    vector2 projected;
    projected.x = (vertex.x / (vertex.z + fov)) * fov;
    projected.y = (vertex.y / (vertex.z + fov)) * fov;
    return projected;
}