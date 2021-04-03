#include "include/shader.h"
#include <glad/glad.h>

void buildShader (Shader* s, char* vertexPath, char* fragmentPath)
{
    unsigned int vertexShader, fragmentShader;
    char vertexStr[MAXCHAR];
    char fragmentStr[MAXCHAR];
    const char* vertexCode;
    const char* fragmentCode;
    unsigned long long i = 0;

    FILE *fp = fopen(vertexPath, "r");
    if (!fp) printf("Failed to open %s\n", vertexPath);
    i = fread(vertexStr, sizeof(char), MAXCHAR, fp);
    vertexStr[i] = '\0';
    fclose(fp);

    fp = fopen(fragmentPath, "r");
    if (!fp) printf("Failed to open %s\n", fragmentPath);
    i = fread(fragmentStr, sizeof(char), MAXCHAR, fp);
    fragmentStr[i] = '\0';
    fclose(fp);

    vertexCode = vertexStr;
    fragmentCode = fragmentStr;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexCode, NULL);
    glCompileShader(vertexShader);
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    if(!success)
    {
        printf("%s ERROR::Shader::VERTEX::COMPILATION_FAILED:\n%s\n", vertexPath, infoLog);
    } else { printf("%s Sucessfully compiled vertex shader.%s\n", vertexPath, infoLog); }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    if(!success)
    {
        printf("%s ERROR::Shader::fragment::COMPILATION_FAILED:\n%s\n", fragmentPath, infoLog);
    } else { printf("%s Sucessfully compiled fragment shader.%s\n", fragmentPath, infoLog); }

    (*s) = glCreateProgram();
    glAttachShader((*s), vertexShader);
    glAttachShader((*s), fragmentShader);
    glLinkProgram((*s));
    glGetProgramiv((*s), GL_LINK_STATUS, &success);
    glGetProgramInfoLog((*s), 512, NULL, infoLog);
    if(!success) {
        printf("ERROR::PROGRAM:\n%s\n", infoLog);
    } else { printf("Sucessfully compiled program. %s\n", infoLog); }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void useShader (Shader s)
{
    glUseProgram(s);
}

void setShaderBool(Shader s, const char* name, bool value)
{
    glUniform1i(glGetUniformLocation(s, name), (int)value);
}
void setShaderInt(Shader s, const char* name, int value)
{
    glUniform1i(glGetUniformLocation(s, name), value);

}
void setShaderFloat(Shader s, const char* name, float value)
{
    glUniform1f(glGetUniformLocation(s, name), value);
}

void setShaderVec3 (Shader s, const char* name, vec3 data)
{
    glUniform3f(glGetUniformLocation(s, name), data[0], data[1], data[2]);
}

void setShaderVec4 (Shader s, const char* name, vec4 data)
{
    glUniform4f(glGetUniformLocation(s, name), data[0], data[1], data[2], data[3]);
}
