#include "include/Player.h"
#include "include/Tabuleiro.h"
#include "include/Camera.h"
#include <GLFW/glfw3.h>

float playerVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

void initPlayer (GameState* gameState)
{
    // Init coords
    setPlayerHeadCoords(gameState, (vec2){PLAYER_START_X, PLAYER_START_Y});
    // Init headPos
    vec3 startPos;
    getTableToWorldPos(gameState->p1->coords[0], gameState->p1->coords[1], &startPos);
    setPlayerHeadPos(gameState, startPos);

    gameState->p1->headDirection = PLAYER_START_DIR;
    gameState->p1->tailSize = 0;
    gameState->p1->speed = PLAYER_START_SPEED;
    gameState->p1->alive = true;
    gameState->p1->scale = PLAYER_START_SCALE;

    // Init direction log
    for (int i=0; i<PLAYER_START_TAILSIZE+1; i++){ // TODO: AQUII
        gameState->p1->directionLog[i] = gameState->p1->headDirection;
    }
    // Init tail array coords
    for (int i=0; i<TAB_WIDTH*TAB_HEIGHT; i++){
        glm_vec2_copy((vec2){-1.0f, -1.0f},gameState->p1->tailCoords[i]);
    }
    // Grow tail accordingly to PLAYER_START_TAILSIZE
    for (int i=0; i<PLAYER_START_TAILSIZE; i++){
        growPlayerTail(gameState);
    }

    /*glm_mat4_copy( (mat4)GLM_MAT4_IDENTITY_INIT, gameState->p1->modelMatrix);
    glm_scale(gameState->p1->modelMatrix, (vec3){gameState->p1->scale, gameState->p1->scale, gameState->p1->scale});*/
    buildShader(&(gameState->p1->shader), "../shaders/player.vs", "../shaders/player.fs");


    glGenVertexArrays(1, &(gameState->p1->VAO));
    glGenBuffers(1, &(gameState->p1->VBO));
    glBindVertexArray((gameState->p1->VAO));
    glBindBuffer(GL_ARRAY_BUFFER, (gameState->p1->VBO));
    glBufferData(GL_ARRAY_BUFFER, sizeof(playerVertices), playerVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
    //glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void getPlayerHeadPos (GameState* gameState, vec3* headPos)
{
    glm_vec3_copy(gameState->p1->headPos, *headPos);
}

void getPlayerHeadDir(GameState* gameState, Player_Movement* dir)
{
    *dir = gameState->p1->headDirection;
}

void getPlayerTailDir(GameState* gameState, Player_Movement* dir)
{
    int tailId = gameState->p1->tailSize;
    *dir =  gameState->p1->directionLog[tailId];
}

void getPlayerHeadCoords(GameState* gameState, vec2* headCoords)
{
    glm_vec2_copy(gameState->p1->coords, *headCoords);
}

void getPlayerTailCoords(GameState* gameState, vec2* tailCoords)
{
    if (getPlayerTailSize(gameState) > 0)
        glm_vec2_copy(gameState->p1->tailCoords[getPlayerTailSize(gameState) - 1], *tailCoords);
    else
        getPlayerHeadCoords(gameState, tailCoords);
}

int getPlayerTailSize (GameState* gameState)
{
    return gameState->p1->tailSize;
}


void calcPlayerCoord (vec2 coord, Player_Movement dir, vec2* destCoord)
{
    switch (dir)
    {
        case Player_Up:
            (*destCoord)[0] = coord[0];
            (*destCoord)[1] = coord[1]-1;
            break;
        case Player_Down:
            (*destCoord)[0] = coord[0];
            (*destCoord)[1] = coord[1]+1;
            break;
        case Player_Left:
            (*destCoord)[0] = coord[0]-1;
            (*destCoord)[1] = coord[1];
            break;
        case Player_Right:
            (*destCoord)[0] = coord[0]+1;
            (*destCoord)[1] = coord[1];
            break;
    }
    // Converts coords to inside board range
    validCoords(*destCoord, destCoord);
}


void movePlayerTail (GameState* gameState)
{
    for (int i=0; i<getPlayerTailSize(gameState); i++) {

        // Direction of tail point starts at the end of the array
        Player_Movement dir = gameState->p1->directionLog[i+1];
        vec2 newCoords;
        calcPlayerCoord(gameState->p1->tailCoords[i], dir, &newCoords);

        glm_vec2_copy(newCoords, gameState->p1->tailCoords[i]);
    }
}

void movePlayerHead (GameState* gameState, Player_Movement dir)
{
    vec3 newWorldPos;
    vec2 headCoords;

    getPlayerHeadCoords(gameState, &headCoords);
    //getTableHeadPos(gameState, &curTableHeadX, &curTableHeadY);

    vec2 newHeadCoords;
    calcPlayerCoord(headCoords, dir, &newHeadCoords);
    setPlayerHeadCoords(gameState, newHeadCoords);

    getTableToWorldPos(newHeadCoords[0], newHeadCoords[1], &newWorldPos);

    setPlayerHeadPos(gameState, newWorldPos);
    //setPlayerModel(gameState->p1, newWorldPos);
}
void setPlayerHeadCoords(GameState* gameState, vec2 headCoords)
{
    glm_vec2_copy(headCoords, (gameState->p1)->coords);
}


void setPlayerHeadDir(GameState* gameState, Player_Movement dir)
{
    Player_Movement opDir;
    getOppositePlayerDir(dir, &opDir);
    // We use directionLog[0] to block fast manuvers like turning the snake backwards combining
    // two directions keys, ex: snake going right, press fast down and then left.
    if ((gameState->p1)->directionLog[0] != opDir){ // bloquear virar para trás
        (gameState->p1)->headDirection = dir;
        //(gameState->p1)->directionLog[0] = dir;
    }
}

void setPlayerHeadPos (GameState* gameState, vec3 headPos)
{
    glm_vec3_copy(headPos, (gameState->p1)->headPos);
}

void setPlayerAlive (GameState* gameState, bool alive)
{
    (gameState->p1)->alive = alive;
}


/*void setPlayerModel (Player* p, vec3 coords)
{
    glm_mat4_identity((*p).modelMatrix);
    glm_scale((*p).modelMatrix, (vec3){(*p).scale, (*p).scale, (*p).scale});
    glm_translate((*p).modelMatrix, coords);
}*/

/*void setPlayerScale (GameState* gameState, float sfactor)
{
    (gameState->p1)->scale = sfactor;
    vec3 curPos;
    getPlayerHeadPos((gameState->p1), &curPos);
    setPlayerModel((gameState->p1), curPos);
}*/

/*void updatePlayerModel (Player* p, vec3 trans)
{
    glm_translate((*p).modelMatrix, trans);
}*/

/*void updatePlayerPos (Player* p, vec3 trans)
{
    vec3 newPos;
    glm_vec3_add((*p).pos, trans, newPos);
    glm_vec3_copy(newPos, (*p).pos);
}*/

void updateDirectionLog (GameState* gameState, Player_Movement curDir)
{
    for (int i=gameState->p1->tailSize; i>0; i--)
        gameState->p1->directionLog[i] = gameState->p1->directionLog[i-1];

    gameState->p1->directionLog[0] = curDir;
}


void logicPlayer (GameState* gameState)
{
        // Move Snake Head
        Player_Movement curDir;
        getPlayerHeadDir(gameState, &curDir);
        movePlayerHead(gameState, curDir);

        // Update directionLog[0] to the "final" headDir snake takes this frame
        gameState->p1->directionLog[0] = curDir;

        // Move Snake Tail
        movePlayerTail(gameState);

        // Update Direction Log
        // directionLog[0] is always the most recent direction and directionLog[gameState->p1->tailSize] is the oldest
        updateDirectionLog(gameState, curDir);

        // Is eating apple?
        if ( isHeadEatingApple(gameState) )
            growPlayerTail(gameState);

        // Verifies if snake ran into himself
        isHeadEatingTail(gameState);
}

void growPlayerTail (GameState* gameState)
{
    // Objective: Find the correct coordinate to where the tail grows
    // Step 1: Find the direction to where it's going to grow
    Player_Movement tailDir, opTailDir;
    getPlayerTailDir(gameState, &tailDir);
    getOppositePlayerDir(tailDir, &opTailDir);

    // Step 2: Find the coordinates of the tail point
    vec2 curTailCoords, newTailCoords;
    getPlayerTailCoords(gameState, &curTailCoords);

    // Step 3: Calculate where the coords of the new tail point
    calcPlayerCoord(curTailCoords, opTailDir, &newTailCoords);

    // Step 4: add coords to tail array and grow tail size variable and add directionLog entry for tail point
    glm_vec2_copy(newTailCoords, gameState->p1->tailCoords[getPlayerTailSize(gameState)]);
    gameState->p1->tailSize++;
    gameState->p1->directionLog[gameState->p1->tailSize] = gameState->p1->directionLog[gameState->p1->tailSize - 1];

}


void drawPlayer (GameState* gameState, mat4* view, mat4* projection)
{
    useShader(gameState->p1->shader);
    unsigned int modelLoc = glGetUniformLocation(gameState->p1->shader, "model");
    unsigned int viewLoc = glGetUniformLocation(gameState->p1->shader, "view");
    unsigned int projectionLoc = glGetUniformLocation(gameState->p1->shader, "projection");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float *)(*view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (float *)(*projection));

    // Set Materials
    setShaderVec3(gameState->p1->shader, "material.ambient", (vec3){1.0f, 0.5f, 0.31f});
    setShaderVec3(gameState->p1->shader, "material.diffuse", (vec3){1.0f, 0.5f, 0.31f});
    setShaderVec3(gameState->p1->shader, "material.specular", (vec3){0.5f, 0.5f, 0.5f});
    setShaderFloat(gameState->p1->shader, "material.shininess", 32.0f);

    glBindVertexArray(gameState->p1->VAO);
    // Draw Head
    setShaderInt(gameState->p1->shader, "isTail", 0);
    setShaderVec3(gameState->p1->shader, "lightPos", (vec3){0.0f, 0.0f, 7.0f});
    vec3 viewPos;
    getCamPos(&viewPos);
    setShaderVec3(gameState->p1->shader, "viewPos", viewPos);

    mat4 headModel = GLM_MAT4_IDENTITY_INIT;
    vec3 headPos;
    getPlayerHeadPos(gameState, &headPos);
    glm_translate(headModel, headPos);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float *)headModel);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Draw Tail
    for(int i=0; i<gameState->p1->tailSize; i++){
        setShaderInt(gameState->p1->shader, "isTail", 1);
        mat4 model = GLM_MAT4_IDENTITY_INIT;
        vec2 tailCoords;
        vec3 tailWorldPos;
        glm_vec2_copy(gameState->p1->tailCoords[i], tailCoords);
        getTableToWorldPos(tailCoords[0], tailCoords[1], &tailWorldPos);

        glm_translate(model, tailWorldPos);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float *)(model));

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void getOppositePlayerDir (Player_Movement dir, Player_Movement* opDir)
{
    switch (dir)
    {
        case Player_Up:
            *opDir = Player_Down;
            break;
        case Player_Down:
            *opDir = Player_Up;
            break;
        case Player_Left:
            *opDir = Player_Right;
            break;
        case Player_Right:
            *opDir = Player_Left;
            break;
    }
}

bool getPlayerAlive (GameState* gameState)
{
    return gameState->p1->alive;
}


void isHeadEatingTail (GameState* gameState)
{
    vec2 headCoords;
    getPlayerHeadCoords(gameState, &headCoords);

    for(int i=0; (i<getPlayerTailSize(gameState)) && getPlayerAlive(gameState); i++){
        if ( (gameState->p1->tailCoords[i])[0] == headCoords[0] &&
             (gameState->p1->tailCoords[i])[1] == headCoords[1] )
        {
            setPlayerAlive(gameState, false);
        }
    }
}

bool isHeadEatingApple (GameState* gameState)
{
    vec2 appleCoords, headCoords;
    getTableAppleCoords(gameState, &appleCoords);
    getPlayerHeadCoords(gameState, &headCoords);
    return ( (appleCoords[0] == headCoords[0]) && (appleCoords[1] == headCoords[1]) );
}