#ifndef SNAKEGL_MESH_H
#define SNAKEGL_MESH_H

#include "../dependencies/cglm/cglm.h"
#include "shader.h"

typedef struct vertex {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
} Vertex;

typedef struct texture {
    unsigned int id;
    char type[32]; // texture_diffuse, texture_specular
    char path[128];
} Texture;

typedef struct mesh {
    Vertex* vertices;
    int V;
    unsigned int* indices;
    int I;
    Texture* textures;
    int T;
    unsigned int VAO, VBO, EBO;
} Mesh;

void initMesh (Mesh* m, Vertex* vertices, int V, unsigned int* indices, int I, Texture* textures, int T);
void drawMesh(Mesh* m, Shader s);
void setupMesh (Mesh* m);

#endif //SNAKEGL_MESH_H
