#ifndef LEARNOPENGL_SHADER_H
#define LEARNOPENGL_SHADER_H

#include <stdio.h>
#include <stdbool.h>
#include "../dependencies/cglm/cglm.h"
#define MAXCHAR 50000

typedef unsigned int Shader;

void buildShader (Shader* s, char* vertexPath, char* fragmentPath);
void useShader (Shader s);
// utility uniform functions
void setShaderBool(Shader s, const char* name, bool value);
void setShaderInt(Shader s, const char* name, int value);
void setShaderFloat(Shader s, const char* name, float value);
void setShaderVec3 (Shader s, const char* name, vec3 data);
void setShaderVec4 (Shader s, const char* name, vec4 data);

#endif //LEARNOPENGL_SHADER_H
