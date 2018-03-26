#include "primlib.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>

///////////////////////////SETTINGS/////////////////////////////////////////////////////

#define HEIGHT 20
#define WIDTH 10
const int FPS_RATE = 15;

////////////////////////////////////////////////////////////////////////////////////////


int gameBoard[HEIGHT][WIDTH] = {0}; /* creating the gameBoard array filled with 0's */
int figureBoard[HEIGHT][WIDTH] = {0}; /* creating the figureBoard array filled with 0's */


////////////////////////////////////////////////////////////////////////////////////////


void screenCleaner(void);
void drawBlock(int, int, int, int, int);
void boardBoundaryDrawer(int, int, int);
void fillBoard(int, int, int);
void testPrintCurrentPiece(int);
void drawFigure(int);
void moveFigure(void);
void moveLeft(void);
void moveRight(void);
int checkCollision(void);
int checkLeftCollision(void);
int checkRightCollision(void);
void figureToGameBoard(void);
void gameOver(void);
void boardDrawer(void);
void gameMove(int*, int*, int*, int*);
void isFigureOnScreen(int);
void whichRotation(int);
void rotate(int, int*);


/* MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN */

int main(int argc, char* argv[])
{
    int posWidth, posHeight, blockSide, centerTheBoard_X, centerTheBoard_Y, /* positioning squares and the board */
    figureOnScreen = 0, /*if figure is on the screen then 1, else 0*/
    rotation = 0, /* which of 4 variants of the figure is on the screen */
    counter = 0,
    whichFigure = 0, /* which of the figures is on the screen */
    score = 0, /* actual score of a player */
    key; /* which key is pressed */
    
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

        
        screenCleaner();
        boardBoundaryDrawer(blockSide, centerTheBoard_X, centerTheBoard_Y);
        
        gameMove(&figureOnScreen, &counter, &whichFigure, &rotation);

        //isFigureOnScreen(figureOnScreen); /* TEST   TEST   TEST   TEST   TEST   TEST   TEST   TEST   TEST   TEST   TEST   TEST   TEST   TEST   TEST   TEST */
        
        fillBoard(blockSide, centerTheBoard_X, centerTheBoard_Y);
        updateScreen();
        
        if ((key = pollkey()) != -1) //checks if any key has been pressed, if no, then false
        {
            if (key == SDLK_ESCAPE)
                exit(3);
            else if (key == SDLK_LEFT)
            {
                if (!checkLeftCollision())
                    moveLeft();
            }
            else if (key == SDLK_RIGHT)
            {
                if (!checkRightCollision())
                    moveRight();
            }
            else if (key == SDLK_DOWN)
            {
                if (!checkCollision())
                    moveFigure();
            }
            else if (key == SDLK_UP)
            {
                boardDrawer();
            }
            else if (key == SDLK_SPACE)
            {
                rotate (whichFigure, &rotation);
            }
        }
        SDL_Delay(1000 / FPS_RATE);
        counter++;
    }
    return 0;
}


/* MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN */


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


void drawFigure(int whichFigure)
{
    int i, j; /* counters */

    for (i = 0; i < 4; i++) /* 4 is a piece side */
        for (j = 0; j < 4; j++) /* 4 is a piece side */
            if (pieces[whichFigure][0][i][j])
                figureBoard[i][WIDTH/2 - 1 + j] = pieces[whichFigure][0][i][j];
            
    //testPrintCurrentPiece(whichFigure); /* TEST   TEST   TEST   TEST   TEST   TEST   TEST   TEST   TEST   TEST   TEST   TEST   TEST   TEST   TEST   TEST */
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


void gameMove(int* figureOnScreen, int* counter, int* whichFigure, int* prevRotation)
{
    if (*counter % (FPS_RATE / 2) == 0)
    {
        *counter %= FPS_RATE / 2;
        if (*figureOnScreen == 0)
        {
            *whichFigure = rand() % 7; /*for selecting a figure*/
            drawFigure(*whichFigure);
            if (checkCollision())
                gameOver();
            *prevRotation = 0;
            *figureOnScreen = 1;
        }
        else if (checkCollision())
        {
            figureToGameBoard();
            *figureOnScreen = 0;
        }
        else
            moveFigure();
    }
}


void rotate(int whichFigure, int* prevRotation)
{
    int i, j, /* counters */
    prevX, prevY, /* position of '2' in a piece before rotation */
    postX, postY, /* position of '2' in a rotated piece */
    deltaX, deltaY, /* differences between position of '2' integer in pieces */
    whereX, whereY, /* top left corner of a rotated piece to draw */
    maxX = 0, maxY = 0, /* maximum x and y !!!indexes!!! in a rotated piece willed with non-zero */
    postRotation, /* index of a rotation of a piece after that rotation */
    collision = 0; /* detector for collisions caused by rotation */
    
    postRotation = *prevRotation + 1;
    postRotation %= 4; /* 4 is a number of variants for each figure */
    //whichRotation(postRotation); /* TEST   TEST   TEST   TEST   TEST   TEST   TEST   TEST   TEST   TEST   TEST   TEST   TEST   TEST   TEST   TEST */
    
    
    for (i = 0; i < HEIGHT; i++)
        for (j = 0; j < WIDTH; j++)
            if (figureBoard[i][j] == 2)
            {
                whereX = j;
                whereY = i;
                i = HEIGHT;
                j = WIDTH;
            }
    
    
    for (i = 0; i < 4; i++) /* 4 is a piece side */
        for (j = 0; j < 4; j++) /* 4 is a piece side */
        {
            if (pieces[whichFigure][*prevRotation][i][j] == 2)
            {
                prevX = j;
                prevY = i;
            }
            if (pieces[whichFigure][postRotation][i][j] == 2)
            {
                postX = j;
                postY = i;
            }
        }
    deltaX = postX - prevX;
    deltaY = postY - prevY;
    //drawRotatedFigure(whereX - deltaX, whereY - deltaY, whichFigure, postRotation);
    
    whereX = whereX - prevX;
    whereY = whereY - prevY;
    
    /* collision detector */
    for (i = 0; i < 4; i++) /* 4 is a piece side */
        for (j = 0; j < 4; j++) /* 4 is a piece side */
                if (gameBoard[whereY - deltaY + i][whereX - deltaX + j])
                    collision = 1;
    
    
    /* maxX and maxY detector */
    for (i = 0; i < 4; i++) /* 4 is a piece side */
        for (j = 0; j < 4; j++) /* 4 is a piece side */
            if (pieces[whichFigure][postRotation][i][j])
            {
                if (maxX < j)
                    maxX = j;
                if (maxY < i)
                    maxY = i;
            }
    
    
    
    if (collision == 0 && whereX >= 0 && whereX <= (WIDTH - maxX - 1) && whereY >= 0 && whereY <= (HEIGHT - maxY - 1))
    {
        for (i = 0; i < HEIGHT; i++)
            for (j = 0; j < WIDTH; j++)
                if (figureBoard[i][j])
                    figureBoard[i][j] = 0;
        
        for (i = 0; i < 4; i++) /* 4 is a piece side */
            for (j = 0; j < 4; j++) /* 4 is a piece side */
            {
                //if (pieces[whichFigure][postRotation][i][j])
                    figureBoard[whereY - deltaY + i][whereX - deltaX + j] = pieces[whichFigure][postRotation][i][j];
            }
    *prevRotation = postRotation;
    }
}


void testPrintCurrentPiece(int whichFigure) {
    printf("=====WHICH FIGURE=====\n");
    printf("Current figure: %d\n", whichFigure);
    printf("=====WHICH FIGURE=====\n");
}


void isFigureOnScreen(int figureOnScreen) {
    printf("=====IS FIGURE ON SCREEN=====\n");
    printf("FigureOnScreen: %d\n", figureOnScreen);
    printf("=====IS FIGURE ON SCREEN=====\n");
}


void whichRotation(int rotation) {
    printf("=====ROTATION=====\n");
    printf("Rotation: %d\n", rotation);
    printf("=====ROTATION=====\n");
}