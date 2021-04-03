#ifndef SNAKEGL_ESTADO_H
#define SNAKEGL_ESTADO_H

#include "../dependencies/cglm/cglm.h"
#include "shader.h"

// Tabuleiro.h
#define TAB_WIDTH 20
#define TAB_HEIGHT 15

#define TAB_START_APPLE_X (TAB_WIDTH / 4)
#define TAB_START_APPLE_Y (TAB_HEIGHT / 4)


typedef enum {
    Casa_Free,
    Casa_Tail,
    Casa_Head,
    Casa_Food
} Table_Casa;

typedef Table_Casa Table[TAB_HEIGHT][TAB_WIDTH];

typedef struct
{
    vec3 pos;
    float scaleX, scaleY, scaleZ;
    mat4 modelMatrix;
    unsigned int VAO, VBO;
    unsigned int texture;
    Shader shader;
} TableDrawingInfo;

// Player.h
#define PLAYER_START_DIR Player_Down
#define PLAYER_START_SPEED 10.0f
#define PLAYER_START_SCALE 1.0f
#define PLAYER_START_TAILSIZE 0
#define PLAYER_START_X (TAB_WIDTH/2)
#define PLAYER_START_Y (TAB_HEIGHT/4)

typedef enum {
    Player_Up,
    Player_Down,
    Player_Left,
    Player_Right
} Player_Movement;

typedef struct player {
    vec2 coords;
    bool alive;
    float speed;
    int tailSize;
    Player_Movement headDirection;
    Player_Movement directionLog[TAB_WIDTH*TAB_HEIGHT]; // directionLog[0] = headDirection
    vec3 headPos;
    vec2 tailCoords[TAB_WIDTH*TAB_HEIGHT];

    //mat4 modelMatrix;
    Shader shader;
    unsigned int VAO, VBO;
    float scale;
} Player;


typedef struct {
    Table* tab;
    TableDrawingInfo* tabD;
    Player* p1;
    int elapsedGameClicks;
} GameState;

void initGameState (GameState* gameState);

#endif //SNAKEGL_ESTADO_H
