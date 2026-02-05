#include "renderer.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

object3d objs[10];
int obj_count = 0;
double fov = 10.0;
int window_width = 800;
int window_height = 600;

void initialize_renderer(int width, int height, double fov_value) {
    window_width = width;
    window_height = height;
    fov = fov_value;
    double vertices[8][3] = {
        {50.0, 50.0, 100.0}, {50.0, 50.0, 0.0}, {50.0, -50.0, 100.0}, {50.0, -50.0, 0.0},
        {-50.0, 50.0, 100.0}, {-50.0, 50.0, 0.0}, {-50.0, -50.0, 100.0}, {-50.0, -50.0, 0.0}
    };
    int edges[12][2] = {
        {0, 1}, {1, 3}, {3, 2}, {2, 0},  // front face
        {4, 5}, {5, 7}, {7, 6}, {6, 4},  // back face
        {0, 4}, {1, 5}, {2, 6}, {3, 7}   // connecting edges
    };
    int faces[6][4] = {
        {0, 1, 3, 2}, // front face
        {4, 5, 7, 6}, // back face
        {0, 1, 5, 4}, // top face
        {2, 3, 7, 6}, // bottom face
        {0, 2, 6, 4}, // left face
        {1, 3, 7, 5}  // right face
    };
    vector3 face_colors[6] = {
        {1.0, 0.0, 0.0}, // red
        {0.0, 1.0, 0.0}, // green
        {0.0, 0.0, 1.0}, // blue
        {1.0, 1.0, 0.0}, // yellow
        {1.0, 0.0, 1.0}, // magenta
        {0.0, 1.0, 1.0}  // cyan
    };
    create_object(vertices, 8, edges, 12, faces, 6, "cube");
}

void create_object(double (*vertices)[3], int vertex_count, int (*edges)[2], int edge_count, int (*faces)[4], int face_count, char* name) {
    object3d obj;
    obj.vertex_count = vertex_count;
    obj.edge_count = edge_count;
    obj.face_count = face_count;
    obj.name = string_create(name);  // Added assignment for the name field

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
    obj.faces = (face*)malloc(sizeof(face) * face_count);
    for (int i = 0; i < face_count; i++) {
        obj.faces[i].v1 = faces[i][0];
        obj.faces[i].v2 = faces[i][1];
        obj.faces[i].v3 = faces[i][2];
        obj.faces[i].v4 = faces[i][3];
    }
    objs[obj_count] = obj; // For simplicity, store in the first slot
    obj_count++;
}

double* get_projected_vertices(int* out_count, char* object_name) {
    object3d obj;
    int found = 0;
    for (int i = 0; i < obj_count; i++) {
        if (string_equals_cstr(&objs[i].name, object_name)) {
            obj = objs[i];
            found = 1;
            break;
        }
    }
    if (!found) {
        *out_count = 0;
        return NULL;
    }
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

int* get_edges(int* out_count, char* object_name) {
    object3d obj;
    int found = 0;
    for (int i = 0; i < obj_count; i++) {
        if (string_equals_cstr(&objs[i].name, object_name)) {
            obj = objs[i];
            found = 1;
            break;
        }
    }
    if (!found) {
        *out_count = 0;
        return NULL;
    }
    *out_count = obj.edge_count * 2; // Each edge has 2 vertices
    int* edges_array = (int*)malloc(sizeof(int) * (*out_count));
    int index = 0;

    for (int i = 0; i < obj.edge_count; i++) {
        edges_array[index++] = obj.edges[i].start;
        edges_array[index++] = obj.edges[i].end;
    }

    return edges_array;
}

double* get_face_projections(int* out_count, char* object_name) {
    object3d obj;
    int found = 0;
    for (int i = 0; i < obj_count; i++) {
        if (string_equals_cstr(&objs[i].name, object_name)) {
            obj = objs[i];
            found = 1;
            break;
        }
    }
    if (!found) {
        *out_count = 0;
        return NULL;
    }
    *out_count = obj.face_count * 8; // Each face has 4 vertices, each vertex has x and y
    double* projected = (double*)malloc(sizeof(double) * (*out_count));
    int index = 0;

    for (int i = 0; i < obj.face_count; i++) {
        face f = obj.faces[i];
        vector3 v1 = obj.vertices[f.v1];
        vector3 v2 = obj.vertices[f.v2];
        vector3 v3 = obj.vertices[f.v3];
        vector3 v4 = obj.vertices[f.v4];

        vector2 p1 = projection(v1);
        vector2 p2 = projection(v2);
        vector2 p3 = projection(v3);
        vector2 p4 = projection(v4);

        projected[index++] = p1.x;
        projected[index++] = p1.y;
        projected[index++] = p2.x;
        projected[index++] = p2.y;
        projected[index++] = p3.x;
        projected[index++] = p3.y;
        projected[index++] = p4.x;
        projected[index++] = p4.y;
    }

    return projected;
}

double* get_face_colors(int* out_count, char* object_name) {
    object3d obj;
    int found = 0;
    for (int i = 0; i < obj_count; i++) {
        if (string_equals_cstr(&objs[i].name, object_name)) {
            obj = objs[i];
            found = 1;
            break;
        }
    }
    if (!found) {
        *out_count = 0;
        return NULL;
    }
    *out_count = obj.face_count * 3; // Each face has 3 color components (r, g, b)
    double* colors = (double*)malloc(sizeof(double) * (*out_count));
    int index = 0;

    for (int i = 0; i < obj.face_count; i++) {
        vector3 c = obj.faces[i].color;
        colors[index++] = c.x;
        colors[index++] = c.y;
        colors[index++] = c.z;
    }

    return colors;
}

void rotate_cube_x(double angle) {
    double center_x = 0.0, center_y = 0.0, center_z = 50.0;
    for (int i = 0; i < objs[0].vertex_count; i++) {
        double x = objs[0].vertices[i].x - center_x;
        double y = objs[0].vertices[i].y - center_y;
        double z = objs[0].vertices[i].z - center_z;
        double new_y = y * cos(angle) - z * sin(angle);
        double new_z = y * sin(angle) + z * cos(angle);
        objs[0].vertices[i].x = x + center_x;
        objs[0].vertices[i].y = new_y + center_y;
        objs[0].vertices[i].z = new_z + center_z;
    }
}

void rotate_cube_y(double angle) {
    double center_x = 0.0, center_y = 0.0, center_z = 50.0;
    for (int i = 0; i < objs[0].vertex_count; i++) {
        double x = objs[0].vertices[i].x - center_x;
        double y = objs[0].vertices[i].y - center_y;
        double z = objs[0].vertices[i].z - center_z;
        double new_x = x * cos(angle) + z * sin(angle);
        double new_z = -x * sin(angle) + z * cos(angle);
        objs[0].vertices[i].x = new_x + center_x;
        objs[0].vertices[i].y = y + center_y;
        objs[0].vertices[i].z = new_z + center_z;
    }
}

void rotate_cube_z(double angle) {
    double center_x = 0.0, center_y = 0.0, center_z = 50.0;
    for (int i = 0; i < objs[0].vertex_count; i++) {
        double x = objs[0].vertices[i].x - center_x;
        double y = objs[0].vertices[i].y - center_y;
        double z = objs[0].vertices[i].z - center_z;
        double new_x = x * cos(angle) - y * sin(angle);
        double new_y = x * sin(angle) + y * cos(angle);
        objs[0].vertices[i].x = new_x + center_x;
        objs[0].vertices[i].y = new_y + center_y;
        objs[0].vertices[i].z = z + center_z;
    }
}

vector2 projection(vector3 vertex) {
    vector2 projected;
    double factor = fov / (fov + vertex.z);
    projected.x = vertex.x * factor + window_width / 2;
    projected.y = -vertex.y * factor + window_height / 2;
    return projected;
}