#include "primlib.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>

///////////////////////////SETTINGS/////////////////////////////////////////////////////

#define HEIGHT 10
#define WIDTH 20
const int FPS_RATE = 10;

////////////////////////////////////////////////////////////////////////////////////////


int gameBoard[HEIGHT][WIDTH] = {0}; /* creating the gameBoard array filled with 0's */
int figureBoard[HEIGHT][WIDTH] = {0}; /* creating the figureBoard array filled with 0's */


////////////////////////////////////////////////////////////////////////////////////////


void screenCleaner(void);
void drawBlock(int, int, int, int, int);
void boardBoundaryDrawer(int, int, int);
void fillBoard(int, int, int);
void testPrintCurrentPiece(int);
void drawFigure(void);
void moveFigure(void);
void moveLeft(void);
void moveRight(void);
int checkCollision(void);
int checkLeftCollision(void);
int checkRightCollision(void);
void figureToGameBoard(void);
void gameOver(void);
void boardDrawer(void);



int main(int argc, char* argv[])
{
    int posWidth, posHeight, blockSide, centerTheBoard_X, centerTheBoard_Y, /* positioning squares */
    figureOnScreen = 0, /*if figure is on the screen then 1, else 0*/
    counter = 0;
    
    srand(time(0));
    
    if(initGraph())
    {
        exit(3);
    }

    posWidth = screenWidth() * 0.9 / WIDTH;
    posHeight = screenHeight() * 0.9 / HEIGHT;
    
    if (posWidth > posHeight) //setting the right size of tetris board blocks
        blockSide = posHeight;
    else if (posWidth < posHeight)
        blockSide = posWidth;
    
    centerTheBoard_X = (screenWidth() - blockSide * WIDTH) / 2;
    centerTheBoard_Y = (screenHeight() - blockSide * HEIGHT) / 2;
    
    while(1)
    {
        if (isKeyDown(SDLK_ESCAPE))
            exit(3);
        if (isKeyDown(SDLK_LEFT))
            if (!checkLeftCollision())
                moveLeft();
        if (isKeyDown(SDLK_RIGHT))
            if (!checkRightCollision())
                moveRight();
        if (isKeyDown(SDLK_DOWN))
            if (!checkCollision())
                moveFigure();
        if (isKeyDown(SDLK_SPACE))
            boardDrawer();
        screenCleaner();
        boardBoundaryDrawer(blockSide, centerTheBoard_X, centerTheBoard_Y);
        if (counter % (FPS_RATE / 2) == 0)
        {
            counter %= FPS_RATE / 2;
            if (figureOnScreen == 0)
            {
                drawFigure();
                if (checkCollision())
                    gameOver();
                figureOnScreen = 1;
            }
            else if (checkCollision())
            {
                figureToGameBoard();
                figureOnScreen = 0;
            }
            else
                moveFigure();
        }
        fillBoard(blockSide, centerTheBoard_X, centerTheBoard_Y);
        updateScreen();
        SDL_Delay(1000 / FPS_RATE);
        counter++;
    }
    return 0;
}



void screenCleaner(void)
{
  filledRect(0, 0, screenWidth() - 1, screenHeight() - 1, YELLOW);
}


void boardBoundaryDrawer(int blockSide, int centerTheBoard_X, int centerTheBoard_Y)
{
    filledRect(centerTheBoard_X, centerTheBoard_Y , centerTheBoard_X + blockSide * WIDTH, centerTheBoard_Y + blockSide * HEIGHT, RED);
}


void drawBlock(int where_x, int where_y, int blockSide, int centerTheBoard_X, int centerTheBoard_Y)
{
    
    filledRect(centerTheBoard_X + where_x * blockSide + 1, centerTheBoard_Y + where_y * blockSide + 1, centerTheBoard_X + (where_x+1) * blockSide - 1, centerTheBoard_Y + (where_y+1) * blockSide - 1, WHITE);
}


void fillBoard(int blockSide, int centerTheBoard_X, int centerTheBoard_Y)
{
    int i, j; /* counters */
    for (i = 0; i < HEIGHT; i++)
        for (j = 0; j < WIDTH; j++)
        {
            if (gameBoard[i][j])
                drawBlock(j, i, blockSide, centerTheBoard_X, centerTheBoard_Y);
            if (figureBoard[i][j])
                drawBlock(j, i, blockSide, centerTheBoard_X, centerTheBoard_Y);
        }
}


void drawFigure(void)
{
    int i, j, /* counters */
    whichFigure = rand() % 7; /*for selecting a figure*/

    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            if (pieces[whichFigure][1][i][j])
                figureBoard[i][WIDTH/2 - 1 + j] = pieces[whichFigure][1][i][j];
            
    testPrintCurrentPiece(whichFigure);
}


void moveFigure(void)
{
    int i, j; /* counters */
    for (i = HEIGHT - 1; i >= 0; i--)
        for (j = WIDTH - 1; j >= 0; j--)
            if (figureBoard[i][j])
            {
                figureBoard[i + 1][j] = figureBoard[i][j];
                figureBoard[i][j] = 0;
            }
}


void moveLeft(void)
{
    int i, j; /* counters */
    for (i = HEIGHT - 1; i >= 0; i--)
        for (j = 0; j < WIDTH; j++)
            if (figureBoard[i][j])
            {
                figureBoard[i][j - 1] = figureBoard[i][j];
                figureBoard[i][j] = 0;
            }
}


void moveRight(void)
{
    int i, j; /* counters */
    for (i = HEIGHT - 1; i >= 0; i--)
        for (j = WIDTH - 1; j >= 0; j--)
            if (figureBoard[i][j])
            {
                figureBoard[i][j + 1] = figureBoard[i][j];
                figureBoard[i][j] = 0;
            }
}


int checkCollision(void) /* 0 => no collision, 1 => collision */
{
    int i, j; /* counters */
    for (i = 0; i < HEIGHT - 1; i++)
        for (j = 0; j < WIDTH; j++)
            if (figureBoard[i][j])
                if (gameBoard[i + 1][j])
                    return 1;
    for (j = 0; j < WIDTH; j++)
        if (figureBoard[HEIGHT - 1][j])
            return 1;
    return 0;
}


int checkLeftCollision(void) /* 0 => no collision, 1 => collision */
{
    int i, j; /* counters */
    for (i = 0; i < HEIGHT; i++)
        for (j = 1; j < WIDTH; j++)
            if (figureBoard[i][j])
                if (gameBoard[i][j - 1])
                    return 1;
    for (i = 0; i < HEIGHT; i++)
        if (figureBoard[i][0])
            return 1;
    return 0;
}


int checkRightCollision(void) /* 0 => no collision, 1 => collision */
{
    int i, j; /* counters */
    for (i = 0; i < HEIGHT; i++)
        for (j = 0; j < WIDTH - 1; j++)
            if (figureBoard[i][j])
                if (gameBoard[i][j + 1])
                    return 1;
    for (i = 0; i < HEIGHT; i++)
        if (figureBoard[i][WIDTH - 1])
            return 1;
    return 0;
}


void figureToGameBoard(void)
{
    int i, j; /* counters */
    for (i = 0; i < HEIGHT; i++)
        for (j = 0; j < WIDTH; j++)
            if (figureBoard[i][j])
            {
                gameBoard[i][j] = figureBoard[i][j];
                figureBoard[i][j] = 0;
            }
}


void gameOver(void)
{
    screenCleaner();
    textout(280, screenHeight() / 2, "GAME OVER", RED);
    updateScreen();
    SDL_Delay(1500);
    textout(280, screenHeight() / 2 + 50, "exit in 3..", RED);
    updateScreen();
    SDL_Delay(1000);
    textout(345, screenHeight() / 2 + 70, "2..", RED);
    updateScreen();
    SDL_Delay(1000);
    textout(345, screenHeight() / 2 + 90, "1..", RED);
    updateScreen();
    SDL_Delay(1000);
    exit(3);
}


void boardDrawer(void)
{
    int i, j; /* counters */
    printf("=====BOARD DRAWER=====\n");
    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
            printf("%2d", gameBoard[i][j]);
        printf("       ");
        for (j = 0; j < WIDTH; j++)
            printf("%2d", figureBoard[i][j]);
        printf("\n");
    }
    printf("=====BOARD DRAWER=====\n");
}


void testPrintCurrentPiece(int whichFigure) {
    printf("=====WHICH FIGURE=====\n");
    printf("Current figure: %d\n", whichFigure);
    printf("=====WHICH FIGURE=====\n");
}