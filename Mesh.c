#include "include/Mesh.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string.h>
#include <stdio.h>


void initMesh (Mesh* m, Vertex* vertices, int V, unsigned int* indices, int I, Texture* textures, int T)
{
    // TODO: responsavel por free()
    m->vertices = vertices;
    m->indices = indices;
    m->textures = textures;
    m->V = V; // number of vertices
    m->I = I; // number of indices
    m->T = T; // number of textures
    setupMesh(m);
}

void drawMesh(Mesh* m, Shader s)
{
    useShader(s);
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for(int i=0; i < m->T; i++) {
        // activate proper texture unit before binding
        glActiveTexture(GL_TEXTURE0 + i);
        // retrieve texture number (the N in diffuse_textureN)
        char result[128];
        char name [128];
        strcpy(name, m->textures[i].type);
        if( strcmp(name, "texture_diffuse") == 0) {
            sprintf(result, "material.%s%d", name, diffuseNr);
            diffuseNr++;
        } else if( strcmp(name, "texture_specular") == 0) {
            sprintf(result, "material.%s%d", name, specularNr);
            specularNr++;
        }

        setShaderInt(s, result, i);
        glBindTexture(GL_TEXTURE_2D, m->textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);
    // draw mesh
    glBindVertexArray(m->VAO);
    glDrawElements(GL_TRIANGLES, m->I, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void setupMesh (Mesh* m)
{
    glGenVertexArrays(1, &m->VAO);
    glGenBuffers(1, &m->VBO);
    glGenBuffers(1, &m->EBO);

    glBindVertexArray(m->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m->VBO);

    glBufferData(GL_ARRAY_BUFFER, m->V * sizeof(Vertex), &(m->vertices[0]), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->I * sizeof(unsigned int),
                 &(m->indices[0]), GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}
