#include "include/Model.h"
#include <stdio.h>
#include <string.h>
#include "dependencies/cglm/cglm.h"
#include "include/Texture.h"

int countMeshes (struct aiNode* node)
{
    int r = node->mNumMeshes;

    for(int i = 0; i < node->mNumChildren; i++) {
        r += countMeshes(node->mChildren[i]);
    }

    return r;
}

void loadModel (Model* m, char* path)
{
    const struct aiScene* scene = aiImportFile( path,aiProcess_Triangulate | aiProcess_FlipUVs);

    // If the import failed, report it
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        printf("ERROR::ASSIMP::%s", aiGetErrorString());
        return;
    } else
        printf("Sucessfully loaded model at path: %s\n", path);

    getDirectoryFromPath(path, m->directory);

    //TODO: TEMPORARY, make a C++ vector
    m->textures_loaded_size = 0;
    m->textures_loaded = malloc(sizeof(Texture) * 10);
    m->mesh_number = 0;
    m->meshes_size = countMeshes(scene->mRootNode);
    m->meshes = malloc(sizeof(Mesh* ) * m->meshes_size);
    for(int i = 0; i < m->meshes_size; i++)
        m->meshes[i] = malloc(sizeof(Mesh));

    processNode (m, scene->mRootNode, scene);
    //aiReleaseImport(scene);
}

void processNode (Model* m, struct aiNode* node, const struct aiScene* scene)
{
    // process all the node's meshes (if any)
    for(int i = 0; i < node->mNumMeshes; i++)
    {
        struct aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        Mesh myMesh = processMesh(m, mesh, scene);
        *(m->meshes[m->mesh_number++]) = myMesh;
    }
    // then do the same for each of its children
    for(int i = 0; i < node->mNumChildren; i++)
    {
        processNode(m, node->mChildren[i], scene);
    }
}

Mesh processMesh(Model* m, struct aiMesh *mesh, const struct aiScene *scene)
{
    Mesh myMesh;
    Vertex* vertices;
    unsigned int* indices;
    Texture* textures;
    int V= mesh->mNumVertices,
        I= 0,
        T= 0;
    // Calcular numero de indices
    for(int i = 0; i < mesh->mNumFaces; i++)
    {
        struct aiFace face = mesh->mFaces[i];
        for(int j = 0; j < face.mNumIndices; j++)
            I++;
    }

    vertices = malloc (sizeof(Vertex) * V);
    indices = malloc (sizeof(unsigned int) * I);


    // Process Vertex
    for(int i=0; i < mesh->mNumVertices; i++)
    {
        Vertex v;
        // process vertex positions
        vec3 vec;
        vec[0] = mesh->mVertices[i].x;
        vec[1] = mesh->mVertices[i].y;
        vec[2] = mesh->mVertices[i].z;
        glm_vec3_copy(vec, v.Position);
        // process vertex normals
        vec[0] = mesh->mNormals[i].x;
        vec[1] = mesh->mNormals[i].y;
        vec[2] = mesh->mNormals[i].z;
        glm_vec3_copy(vec, v.Normal);
        // and process texture coordinates
        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            vec2 tmp;
            tmp[0] = mesh->mTextureCoords[0][i].x;
            tmp[1] = mesh->mTextureCoords[0][i].y;
            glm_vec3_copy(tmp, v.TexCoords);
        }
        else
            glm_vec3_copy((vec2){0.0f, 0.0f}, v.TexCoords);

        vertices[i] = v; // :TODO sera que foi feita uma copia ou sera que se perde a memoria?
    }
    // Process Indices
    int k=0;
    for(int i = 0; i < mesh->mNumFaces; i++)
    {
        struct aiFace face = mesh->mFaces[i];
        for(int j = 0; j < face.mNumIndices; j++)
            indices[k++] = face.mIndices[j];
    }
    // Process Material
    if(mesh->mMaterialIndex >= 0)
    {
        struct aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        // Diffuse
        unsigned int numDifMaps = aiGetMaterialTextureCount(material, aiTextureType_DIFFUSE);
        Texture* diffuseMaps = loadMaterialTextures(m, material, aiTextureType_DIFFUSE, "texture_diffuse");
        // Specular
        unsigned int numSpecMaps = aiGetMaterialTextureCount(material, aiTextureType_SPECULAR);
        Texture* specularMaps = loadMaterialTextures(m, material, aiTextureType_SPECULAR, "texture_specular");

        T = (numDifMaps + numSpecMaps);
        textures = malloc(sizeof(Texture) * (numDifMaps + numSpecMaps));
        memcpy(textures, diffuseMaps, numDifMaps*sizeof(Texture));
        memcpy(textures + numDifMaps, specularMaps, numSpecMaps*sizeof(Texture));

        free(diffuseMaps);
        free(specularMaps);

    }

    initMesh(&myMesh, vertices, V, indices, I, textures, T);
    return myMesh;
}

Texture* loadMaterialTextures(Model* m, struct aiMaterial *mat, enum aiTextureType type, char* typeName)
{
    unsigned int T = aiGetMaterialTextureCount(mat, type); // N of textures
    Texture* textures = malloc(sizeof(Texture) * T);

    for(int i=0; i < T; i++)
    {
        struct aiString str;
        aiGetMaterialTexture(mat, type, i, &str, NULL, NULL, NULL, NULL, NULL, NULL);

        // Check if texture was already loaded before
        bool skip = false;
        for(int j = 0; j < m->textures_loaded_size; j++) {
            if (strcmp(m->textures_loaded[j].path, str.data) == 0) {
                textures[i] = m->textures_loaded[j];
                skip = true;
                break;
            }
        }
        if (!skip) {
            Texture texture;
            texture.id = loadTexture(m, str.data, m->directory);
            strcpy(texture.type, typeName);
            strcpy(texture.path, str.data);
            textures[i] = texture; // TODO: faz copia?
            m->textures_loaded[m->textures_loaded_size++] = texture;
        }

    }
    return textures;
}


void drawModel (Model* m, Shader s)
{
    for(int i = 0; i<m->meshes_size; i++){
        drawMesh( ((m->meshes)[i]), s);
    }
}

void getDirectoryFromPath (char* path, char* dest)
{
    int n = strlen(path);
    char* cpy;
    cpy = strdup(path);

    // Remover tudo ate encontrar primeiro '/' (a começar no fim da string)
    int i;
    for(i = n-1; i>0 && path[i] != '/'; i--);

    cpy[i+1] = '\0';

    strcpy(dest, cpy);
}
