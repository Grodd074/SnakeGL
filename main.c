#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "include/shader.h"
#include "include/Camera.h"
#include "include/Player.h"

#define STB_IMAGE_IMPLEMENTATION
#include "dependencies/stb_image.h"

#include "dependencies/cglm/cglm.h"
#include "include/Estado.h"
#include "include/Tabuleiro.h"
#include "include/Model.h"


GameState* gameState;
vec4 lightPos;

/*float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
};*/

float tableVertices[] = {
        0.5f, 0.5f, 0.0f, // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,   // bottom left
        -0.5f, 0.5f, 0.0f   // top left
};


unsigned int tableIndices[] = {
        0, 1, 3,
        1, 2, 3 // second triangle // first triangl
};

/*float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};*/

float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

vec3 cubePositions[] = {
        ((vec3) { 0.0f,  0.0f,  0.0f}),
        ((vec3) { 2.0f,  5.0f, -15.0f}),
        ((vec3) {-1.5f, -2.2f, -2.5f}),
        ((vec3) {-3.8f, -2.0f, -12.3f}),
        ((vec3) { 2.4f, -0.4f, -3.5f}),
        ((vec3) {-1.7f,  3.0f, -7.5f}),
        ((vec3) { 1.3f, -2.0f, -2.5f}),
        ((vec3) { 1.5f,  2.0f, -2.5f}),
        ((vec3) { 1.5f,  0.2f, -1.5f}),
        ((vec3) {-1.3f,  1.0f, -1.5f})
};

// positions of the point lights
vec3 pointLightPositions[] = {
        ((vec3) { 0.7f,  0.2f,  2.0f}),
        ((vec3) { 2.3f, -3.3f, -4.0f}),
        ((vec3) {-4.0f,  2.0f, -12.0f}),
        ((vec3) {0.0f,  0.0f, -3.0f})
};


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    processCamMouse(xpos, ypos, true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    processCamMouseScroll(yoffset);
}

float lastFrame = 0;
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float currentFrame = glfwGetTime();
    float deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        processCamKeyboard(Camera_Forward, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        processCamKeyboard(Camera_Backward, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        processCamKeyboard(Camera_Left, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        processCamKeyboard(Camera_Right, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
        lightPos[2] -= 0.1f;
    if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS)
        lightPos[2] += 0.1f;
    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
        lightPos[0] -= 0.1f;
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
        lightPos[0] += 0.1f;
    if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS) // up
        lightPos[1] += 0.1f;
    if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS)
        lightPos[1] -= 0.1f;

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        setPlayerHeadDir(gameState, Player_Up);
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        setPlayerHeadDir(gameState, Player_Down);
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        setPlayerHeadDir(gameState, Player_Left);
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        setPlayerHeadDir(gameState, Player_Right);

    if (key == GLFW_KEY_I && action == GLFW_PRESS)
        growPlayerTail(gameState);
    /*if (key == GLFW_KEY_O && action == GLFW_PRESS)
        setPlayerScale(gameState, 1.0f);
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
        setPlayerScale(gameState, 4.0f);*/

    if (key == GLFW_KEY_T && action == GLFW_PRESS)
        printTable(gameState);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(800, 800, "SnakeGL", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // CALLBACKS
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    gameState = malloc(sizeof(GameState));
    initGameState(gameState);

    // Depth
    glEnable(GL_DEPTH_TEST);
    // glDepthMask(GL_FALSE); // temporary read-only depth buffer
    glDepthFunc(GL_LESS);

    // Stencil
    glEnable(GL_STENCIL_TEST);
    // glStencilMask(0xFF); // each bit is written to the stencil buffer as is
    // glStencilMask(0x00); // each bit ends up as 0 in the stencil buffer (disabling writes)

    // Loading Shaders
    Shader shaderProgram, lightShader, outlineShader;
    buildShader(&shaderProgram, "../shaders/shader.vs", "../shaders/shader.fs");
    buildShader(&lightShader, "../shaders/light.vs", "../shaders/light.fs");
    buildShader(&outlineShader, "../shaders/outline.vs", "../shaders/outline.fs");

    // LOAD MODEL
    Model m, coneModel;
    loadModel(&m, "C:/Users/Duarte/CLionProjects/SnakeGL/assets/guitar-backpack/backpack.obj");
    loadModel(&coneModel, "C:/Users/Duarte/CLionProjects/SnakeGL/assets/cone/wood_cone.obj");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    //glEnableVertexAttribArray(1);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    // Light VAO

    glm_vec4_copy((vec4){11.5f, -5.5f, 28.0f, 1.0f}, lightPos); // when w=0.0 light is a direction
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // we only need to bind to the VBO, the container's VBO's data already contains the data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("../assets/container2.png", &width, &height, &nrChannels, 0);
    unsigned int diffuseMap, specularMap;
    glGenTextures(1, &diffuseMap); glGenTextures(1, &specularMap);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else { printf("failed to load texture\n"); }

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../assets/container2_specular.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else { printf("failed to load texture\n"); }


    processCamMouse(400, 400, true); // para processar logo no inicio
    // 400 300 essencial, meio do ecra
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while(!glfwWindowShouldClose(window) && gameState->p1->alive)
    {
        processInput(window);

        glEnable(GL_DEPTH_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glStencilMask(0x00);

        mat4 view = GLM_MAT4_IDENTITY_INIT;
        mat4 projection = GLM_MAT4_IDENTITY_INIT;

        getCamViewMatrix(&view);
        glm_perspective(glm_rad(getCamZoom()), 800.0f/800.0f, 0.1f, 100.0f, projection);

        useShader(lightShader);
        unsigned lightModelLoc = glGetUniformLocation(lightShader, "model");
        unsigned int lightViewLoc = glGetUniformLocation(lightShader, "view");
        unsigned int lightProjectionLoc = glGetUniformLocation(lightShader, "projection");
        useShader(shaderProgram);
        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float *)view);
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (float *)projection);
        useShader(lightShader);
        glUniformMatrix4fv(lightViewLoc, 1, GL_FALSE, (float *)view);
        glUniformMatrix4fv(lightProjectionLoc, 1, GL_FALSE, (float *)projection);
        useShader(outlineShader);
        unsigned int outlineModelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int outlineViewLoc = glGetUniformLocation(shaderProgram, "view");
        unsigned int outlineProjectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(outlineViewLoc, 1, GL_FALSE, (float *)view);
        glUniformMatrix4fv(outlineProjectionLoc, 1, GL_FALSE, (float *)projection);

        // Game LOOP
        if ( (gameState->p1->speed*glfwGetTime() - gameState->elapsedGameClicks) > 1) {
            gameState->elapsedGameClicks++;

            // Logic
            logicPlayer(gameState);


            logicTable(gameState);
        }
        // Render Player
        drawPlayer(gameState, &view, &projection);
        // Render Table
        drawTable(gameState, &view, &projection);


        // Render Light Cube
        useShader(lightShader);
        glBindVertexArray(lightVAO);

        for(int i=0; i < 4; i++) {
            mat4 lightModel = GLM_MAT4_IDENTITY_INIT;
            glm_translate(lightModel, pointLightPositions[i]);
            glm_scale(lightModel, (vec3){0.3f, 0.3f, 0.3f});
            glUniformMatrix4fv(lightModelLoc, 1, GL_FALSE, (float *)lightModel);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Render Happy Cube

        useShader(shaderProgram);
        glBindVertexArray(VAO);
        mat4 cubeModel = GLM_MAT4_IDENTITY_INIT;
        //glm_scale(cubeModel, (vec3){0.75f, 0.3f, 0.2f});
        setShaderVec4(shaderProgram, "light.position", lightPos);
        vec3 camPos, camFront;
        getCamPos(&camPos);
        getCamFront(&camFront);
        setShaderVec3(shaderProgram, "viewPos", camPos);

        // Set Lights
        // Direction Light
        setShaderVec3(shaderProgram, "dirLight.direction",(vec3){-0.5f, -1.0f, -0.4f});
        setShaderVec3(shaderProgram, "dirLight.ambient",(vec3){0.2f, 0.2f, 0.2f});
        setShaderVec3(shaderProgram,"dirLight.diffuse",  (vec3){0.8f, 0.8f, 0.8f});
        setShaderVec3(shaderProgram,"dirLight.specular", (vec3){1.0f, 1.0f, 1.0f});
        // Point Lights
        // point light 1
        setShaderVec3(shaderProgram, "pointLights[0].position", pointLightPositions[0]);
        setShaderVec3(shaderProgram,"pointLights[0].ambient", (vec3){0.05f, 0.05f, 0.05f});
        setShaderVec3(shaderProgram,"pointLights[0].diffuse", (vec3){0.8f, 0.8f, 0.8f});
        setShaderVec3(shaderProgram,"pointLights[0].specular", (vec3){1.0f, 1.0f, 1.0f});
        setShaderFloat(shaderProgram,"pointLights[0].constant", 1.0f);
        setShaderFloat(shaderProgram,"pointLights[0].linear", 0.09);
        setShaderFloat(shaderProgram,"pointLights[0].quadratic", 0.032);
        // point light 2
        setShaderVec3(shaderProgram,"pointLights[1].position", pointLightPositions[1]);
        setShaderVec3(shaderProgram,"pointLights[1].ambient", (vec3){0.05f, 0.05f, 0.05f});
        setShaderVec3(shaderProgram,"pointLights[1].diffuse", (vec3){0.8f, 0.8f, 0.8f});
        setShaderVec3(shaderProgram,"pointLights[1].specular", (vec3){1.0f, 1.0f, 1.0f});
        setShaderFloat(shaderProgram,"pointLights[1].constant", 1.0f);
        setShaderFloat(shaderProgram,"pointLights[1].linear", 0.09);
        setShaderFloat(shaderProgram,"pointLights[1].quadratic", 0.032);
        // point light 3
        setShaderVec3(shaderProgram,"pointLights[2].position", pointLightPositions[2]);
        setShaderVec3(shaderProgram,"pointLights[2].ambient", (vec3){0.05f, 0.05f, 0.05f});
        setShaderVec3(shaderProgram,"pointLights[2].diffuse", (vec3){0.8f, 0.8f, 0.8f});
        setShaderVec3(shaderProgram,"pointLights[2].specular", (vec3){1.0f, 1.0f, 1.0f});
        setShaderFloat(shaderProgram,"pointLights[2].constant", 1.0f);
        setShaderFloat(shaderProgram,"pointLights[2].linear", 0.09);
        setShaderFloat(shaderProgram,"pointLights[2].quadratic", 0.032);
        // point light 4
        setShaderVec3(shaderProgram,"pointLights[3].position", pointLightPositions[3]);
        setShaderVec3(shaderProgram,"pointLights[3].ambient", (vec3){0.05f, 0.05f, 0.05f});
        setShaderVec3(shaderProgram,"pointLights[3].diffuse", (vec3){0.8f, 0.8f, 0.8f});
        setShaderVec3(shaderProgram,"pointLights[3].specular", (vec3){1.0f, 1.0f, 1.0f});
        setShaderFloat(shaderProgram,"pointLights[3].constant", 1.0f);
        setShaderFloat(shaderProgram,"pointLights[3].linear", 0.09);
        setShaderFloat(shaderProgram,"pointLights[3].quadratic", 0.032);
        // SpotLight
        setShaderVec3(shaderProgram, "spotLight.position", camPos);
        setShaderVec3(shaderProgram, "spotLight.direction", camFront);
        setShaderVec3(shaderProgram, "spotLight.ambient", (vec3){0.0f, 0.0f, 0.0f});
        setShaderVec3(shaderProgram, "spotLight.diffuse", (vec3){1.0f, 1.0f, 1.0f});
        setShaderVec3(shaderProgram, "spotLight.specular", (vec3){1.0f, 1.0f, 1.0f});
        setShaderFloat(shaderProgram, "spotLight.constant", 1.0f);
        setShaderFloat(shaderProgram, "spotLight.linear", 0.09f);
        setShaderFloat(shaderProgram, "spotLight.quadratic", 0.032f);
        setShaderFloat(shaderProgram, "spotLight.cutOff", cos(glm_rad(12.5f)));
        setShaderFloat(shaderProgram, "spotLight.outerCutOff", cos(glm_rad(15.0f)));


        // Set Materials
        setShaderInt(shaderProgram, "material.texture_diffuse1", 0);
        setShaderInt(shaderProgram, "material.texture_specular1", 1);
        setShaderFloat(shaderProgram, "material.shininess", 32.0f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float *)cubeModel);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        for(int i=0; i < 10; i++) {
            glm_mat4_identity(cubeModel);
            //glm_translate(cubeModel, (vec3){9.5f, -7.5f, 21.0f});
            glm_translate(cubeModel, cubePositions[i]);
            float angle = 20.0f * i;
            glm_rotate(cubeModel, glm_rad(angle), (vec3){1.0f, 0.3f, 0.5f} );
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float *)cubeModel);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        //glStencilMask(0x00);
        //glDisable(GL_DEPTH_TEST);
        useShader(outlineShader);
        glBindVertexArray(lightVAO);
        for(int i=0; i < 10; i++) {
            glm_mat4_identity(cubeModel);
            //glm_translate(cubeModel, (vec3){9.5f, -7.5f, 21.0f});
            glm_translate(cubeModel, cubePositions[i]);
            float angle = 20.0f * i;
            glm_rotate(cubeModel, glm_rad(angle), (vec3){1.0f, 0.3f, 0.5f} );
            //glm_scale(cubeModel, (vec3){1.3f, 1.3f, 1.3f});

            glUniformMatrix4fv(outlineModelLoc, 1, GL_FALSE, (float *)cubeModel);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        //glStencilMask(0xFF);
        //glStencilFunc(GL_ALWAYS, 1, 0xFF);
        //glEnable(GL_DEPTH_TEST);

        useShader(shaderProgram);
        mat4 model = GLM_MAT4_IDENTITY_INIT;
        glm_translate(model, (vec3){10.0f, 10.0f, 10.0f});
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float *)model);
        drawModel(&m, shaderProgram);

        glm_mat4_identity(model);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float *)model);
        drawModel(&coneModel, shaderProgram);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}