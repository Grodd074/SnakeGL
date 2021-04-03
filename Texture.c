#include "include/Texture.h"
#include <string.h>

#include <glad/glad.h>

#include "dependencies/stb_image.h"

unsigned int loadTexture (Model* m, char* filename, char* directory) {
    int width, height, nrChannels;

    // Calculates file extension
    char* fileExtension = strchr(filename, '.');

    // Calculates full path
    char* cpy_dir = malloc(strlen(directory) + strlen(filename) + 1);
    strcpy (cpy_dir, directory);
    stbi_set_flip_vertically_on_load(true);

    unsigned char *data = stbi_load(strcat(cpy_dir, filename), &width, &height, &nrChannels, 0);
    unsigned int texture;

    glGenTextures(1, &texture);
    // TODO: GL_TEXTURE0?
    glActiveTexture(GL_TEXTURE0 + m->textures_loaded_size);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data)
    {
        if ( strcmp(fileExtension, ".png") == 0 ) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    } else { printf("failed to load texture at %s\n", cpy_dir); }

    free(cpy_dir);
    return texture;
}
