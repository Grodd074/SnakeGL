#include "include/Estado.h"

#include "include/Player.h"
#include "include/Tabuleiro.h"

void initGameState (GameState* gameState)
{
    gameState->elapsedGameClicks = 0;
    gameState->tab = malloc(sizeof(Table));
    gameState->p1 = malloc(sizeof(Player));
    gameState->tabD = malloc(sizeof(TableDrawingInfo));

    initPlayer(gameState);
    initTable(gameState);
    initTableDrawingInfo(gameState->tabD);
}
