#ifndef SNAKEGL_TABULEIRO_H
#define SNAKEGL_TABULEIRO_H

#include "../dependencies/cglm/cglm.h"
#include "Estado.h"

void initTable(GameState* gameState);
void initTableDrawingInfo(TableDrawingInfo * td);

void getTableAppleCoords (GameState* gameState, vec2* coords);
void getTableToWorldPos(int x, int y, vec3* dest);

void generateTableAppleCoords(GameState* gameState, vec2* coords);
void spawnAppleTable (GameState* gameState);
void clearTable (GameState* gameState);
void updateTable (GameState* gameState);

void logicTable (GameState* gameState);
void drawTable (GameState* gameState, mat4* view, mat4* projection);
void printTable (GameState* gameState);

bool occupiedPositionTable (GameState* gameState, vec2 coords);
void validCoords (vec2 coords, vec2* destCoords);

#endif //SNAKEGL_TABULEIRO_H
