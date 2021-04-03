#include "include/Tabuleiro.h"
#include <glad/glad.h>

#include "dependencies/stb_image.h"
#include "include/Player.h"

float vertices2[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // cubo detras: canto inferior esq
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  // cubo detras: canto inferior dir
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  // cubo detras: canto superior dir
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  // cubo detras: canto superior dir
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // cubo detras: canto superior esq
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // cubo detras: canto inferior esq

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // cubo frente: canto inferior esq
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  // cubo frente: canto inferior dir
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  // cubo frente: canto superior dir
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  // cubo frente: canto superior dir
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // cubo frente: canto superior esq
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // cubo frente: canto inferior esq

        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
};

void initTable(GameState* gameState)
{
    // tabuleiro tudo a free
    clearTable(gameState);

    (*gameState->tab)[TAB_START_APPLE_Y][TAB_START_APPLE_X] = Casa_Food;

    //spawnAppleTable(gameState);
    updateTable(gameState);

}

void initTableDrawingInfo(TableDrawingInfo * td)
{
    glGenTextures(1, &(td->texture));
    glGenVertexArrays(1, &(td->VAO));
    glGenBuffers(1, &(td->VBO));
    glBindVertexArray(td->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, td->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    glm_mat4_identity(td->modelMatrix);
    glm_vec3_copy((vec3){0.5f, -0.5f, 0.0f}, td->pos);
    //td->scaleX = 10.0f;
    //td->scaleY = 7.5f;
    //td->scaleZ = 0.25f;
    td->scaleX = 0.1f;
    td->scaleY = 0.1f;
    td->scaleZ = 0.1f;

    glm_scale(td->modelMatrix, (vec3){td->scaleX, td->scaleY, td->scaleZ});
    //glm_translate(td->modelMatrix, td->pos);

    buildShader(&(td->shader), "../shaders/tableShader.vs", "../shaders/tableShader.fs");

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, td->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    unsigned char *data = stbi_load("../assets/board.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else { printf("failed to load texture\n"); }

    useShader(td->shader);
    setShaderInt(td->shader, "gridTexture", 2);
    useShader(0);
}


void getTableToWorldPos(int x, int y, vec3* dest)

{
    // De acordo com os vertices do cubo definidos manualmente, para mover uma posiçao inteira
    // temos que fazer uma translaçao de 2*(*p).scale

    //printf("x: %d\n", x);
    //printf("y: %d\n", y);

    // Como e primeiro feita a escala e depois a translaçao, o jogador mexe-se dependendo do seu tamanho
    vec3 res = {x, -y, 0.0f}; // 3.0f offset eixo z

    glm_vec3_copy(res, (*dest));
}

void drawTable (GameState* gameState, mat4* view, mat4* projection)
{
    useShader(gameState->tabD->shader);
    unsigned int modelLocTab = glGetUniformLocation(gameState->tabD->shader, "model");
    unsigned int viewLocTab = glGetUniformLocation(gameState->tabD->shader, "view");
    unsigned int projectionLocTab = glGetUniformLocation(gameState->tabD->shader, "projection");
    mat4 modelTab = GLM_MAT4_IDENTITY_INIT;

    //glm_scale(modelTab, (vec3){5.0f, 5.0f, 0.33f});


    //glUniformMatrix4fv(modelLocTab, 1, GL_FALSE, (float *)gameState->tabD->modelMatrix);
    glUniformMatrix4fv(viewLocTab, 1, GL_FALSE, (float *)view);
    glUniformMatrix4fv(projectionLocTab, 1, GL_FALSE, (float *)projection);

    //glActiveTexture(GL_TEXTURE2);
    //glBindTexture(GL_TEXTURE_2D, gameState->tabD->texture);

    glBindVertexArray(gameState->tabD->VAO);

    vec3 pos;
    for(int i=0; i<TAB_HEIGHT; i++){
        for(int j=0; j<TAB_WIDTH; j++){
            mat4 model = GLM_MAT4_IDENTITY_INIT;
            getTableToWorldPos(j, i, &pos);
            glm_translate(model, pos);

            if ((*gameState->tab)[i][j] == Casa_Food){
                setShaderInt(gameState->tabD->shader, "isApple", 1);
                glm_scale(model, (vec3){gameState->tabD->scaleX*2, gameState->tabD->scaleY*2, gameState->tabD->scaleZ*2});

            }
            else{
                setShaderInt(gameState->tabD->shader, "isApple", 0);
                glm_scale(model, (vec3){gameState->tabD->scaleX, gameState->tabD->scaleY, gameState->tabD->scaleZ});
            }

            glUniformMatrix4fv(modelLocTab, 1, GL_FALSE, (float *)model);
            glDrawArrays(GL_TRIANGLES, 0, 36);

        }
    }
}

void validCoords (vec2 coords, vec2* destCoords)
{
    if(coords[0] < 0) (*destCoords)[0] = TAB_WIDTH-1;
    if(coords[0] >= TAB_WIDTH) (*destCoords)[0] = 0;
    if(coords[1] < 0) (*destCoords)[1] = TAB_HEIGHT-1;
    if(coords[1] >= TAB_HEIGHT) (*destCoords)[1] = 0;
}

void printTable (GameState* gameState)
{
    printf("--------------------\n");
    for(int i=0; i<TAB_HEIGHT; i++){
        for(int j=0; j<TAB_WIDTH; j++){
            switch( (*gameState->tab)[i][j] )
            {
                case Casa_Free:
                    printf("O");
                    break;
                case Casa_Food:
                    printf("F");
                    break;
                case Casa_Tail:
                    printf("T");
                    break;
                case Casa_Head:
                    printf("H");
            }
        }
        printf("\n");
    }
    printf("--------------------\n");

}
bool occupiedPositionTable (GameState* gameState, vec2 coords)
{
    int x = coords[0], y = coords[1];
    return ( (*gameState->tab)[x][y] != Casa_Free );
}

void generateTableAppleCoords(GameState* gameState, vec2* coords)
{
    int randX, randY;
    do
    {
        randX = rand() % TAB_WIDTH;
        randY = rand() % TAB_HEIGHT;
    } while ( occupiedPositionTable(gameState, (vec2){randX, randY}) );
    (*coords)[0] = randX;
    (*coords)[1] = randY;
}

void spawnAppleTable (GameState* gameState)
{
    int x,y;
    vec2 randCoords;
    generateTableAppleCoords(gameState, &randCoords);
    x = randCoords[0];
    y = randCoords[1];
    (*gameState->tab)[y][x] = Casa_Food;
}


void getTableAppleCoords (GameState* gameState, vec2* coords)
{
    (*coords)[0] = -1;
    (*coords)[1] = -1;

    for(int i=0; i<TAB_HEIGHT; i++){
        for (int j = 0; j < TAB_WIDTH; j++){
            if ((*gameState->tab)[i][j] == Casa_Food){
                (*coords)[0] = j;
                (*coords)[1] = i;
            }
        }
    }
}


void clearTable (GameState* gameState)
{
    for(int i=0; i<TAB_HEIGHT; i++)
        for (int j = 0; j < TAB_WIDTH; j++)
            (*gameState->tab)[i][j] = Casa_Free;
}

void updateTable (GameState* gameState)
{
    vec2 appleCoords;
    vec2 headCoords;
    //vec2 tailCoord;
    int x,y;

    // Guardar coords comida ou gerar novas
    if ( !isHeadEatingApple(gameState) )
        getTableAppleCoords (gameState, &appleCoords);
    else
        generateTableAppleCoords( gameState, &appleCoords );

    // Clear Buffer
    clearTable(gameState);

    // Update Head
    getPlayerHeadCoords(gameState, &headCoords);
    x = headCoords[0];
    y = headCoords[1];
    (*gameState->tab)[y][x] = Casa_Head;

    // Update Tail
    for(int i=0; i<getPlayerTailSize(gameState); i++)
    {
        vec2 tailCoord;
        glm_vec2_copy(gameState->p1->tailCoords[i], tailCoord);
        x = tailCoord[0];
        y = tailCoord[1];
        (*gameState->tab)[y][x] = Casa_Tail;
    }

    // Put Apple
    x = appleCoords[0];
    y = appleCoords[1];
    (*gameState->tab)[y][x] = Casa_Food;
}

void logicTable (GameState* gameState)
{
    // Update table based on head and tail coords
    updateTable(gameState);
}

