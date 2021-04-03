#ifndef SNAKEGL_MODEL_H
#define SNAKEGL_MODEL_H

#include "Mesh.h"
#include "../dependencies/assimp/include/cimport.h"        // Plain-C interface
#include "../dependencies/assimp/include/scene.h"          // Output data structure
#include "../dependencies/assimp/include/postprocess.h"    // Post processing flags


#define MAX_DIRECTORY_SIZE 256

typedef struct Model {
    Mesh** meshes;
    int meshes_size; // number of meshes
    char directory[MAX_DIRECTORY_SIZE];
    Texture* textures_loaded;
    int textures_loaded_size;
    int mesh_number;
} Model;

void loadModel (Model* m, char* path);
void drawModel (Model* m, Shader s);
void processNode (Model* m,struct aiNode* node, const struct aiScene* scene);
Mesh processMesh(Model* m, struct aiMesh *mesh, const struct aiScene *scene);
Texture* loadMaterialTextures(Model* m, struct aiMaterial *mat, enum aiTextureType type, char* typeName);

// auxiliar
void getDirectoryFromPath (char* path, char* dest);
int countMeshes (struct aiNode* node);

#endif //SNAKEGL_MODEL_H
