#ifndef SNAKEGL_PLAYER_H
#define SNAKEGL_PLAYER_H

#include "../dependencies/cglm/cglm.h"
#include <glad/glad.h>

#include "Estado.h"

void initPlayer (GameState* gameState);

void movePlayerTail (GameState* gameState);
void movePlayerHead (GameState* gameState, Player_Movement dir);

void getPlayerHeadDir(GameState* gameState, Player_Movement* dir);
void getPlayerTailDir(GameState* gameState, Player_Movement* dir);
void getPlayerHeadCoords(GameState* gameState, vec2* headCoords);
void getPlayerTailCoords(GameState* gameState, vec2* tailCoords);
void getPlayerHeadPos (GameState* gameState, vec3* headPos);
int getPlayerTailSize (GameState* gameState);
bool getPlayerAlive (GameState* gameState);

void setPlayerAlive (GameState* gameState, bool alive);
void setPlayerHeadDir(GameState* gameState, Player_Movement dir);
void setPlayerHeadPos (GameState* gameState, vec3 headPos);
void setPlayerHeadCoords(GameState* gameState, vec2 headCoords);
//void setPlayerModel (Player* p, vec3 coords);
//void setPlayerScale (GameState* gameState, float sfactor);

//void updatePlayerPos (Player* p, vec3 trans);
//void updatePlayerModel (Player* p, vec3 trans);
void updateDirectionLog (GameState* gameState, Player_Movement curDir);

void logicPlayer (GameState* gameState);
void drawPlayer (GameState* gameState, mat4* view, mat4* projection);
void growPlayerTail (GameState* gameState);

// Aux
void getOppositePlayerDir (Player_Movement dir, Player_Movement* opDir);
void calcPlayerCoord (vec2 coord, Player_Movement dir, vec2* destCoord);
void isHeadEatingTail (GameState* gameState);
bool isHeadEatingApple (GameState* gameState);

#endif //SNAKEGL_PLAYER_H
