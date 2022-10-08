#include <3ds.h>
#include <algorithm>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

bool gameOver;
const int width = 20;
const int height = 20;
int snakeX, snakeY, fruitX, fruitY, score, frame;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;
int nTail;

void drawCharXY(int x, int y, char s) {
    printf("\x1b[%i;%iH%c", y+1, x+1, s);
}

void seedFruit() {
    fruitX = 1 + rand() % (width-1);
    fruitY = 1 + rand() % (height-1);
}

bool speedFormula() {
    return (frame % max(2, (int)(10 - round((score / 10)))) == 0);
}

void Setup()
{
    gameOver = false;
    dir = STOP;
    snakeX = width / 2;
    snakeY = height / 2;

    seedFruit();

    score = 0;
}

void Draw()
{
    for(int x = 0; x <= width; x++) {
        for(int y = 0; y <= height; y++) {
            if (x == 0 || y == 0 || x == width || y == height)
                drawCharXY(x, y, '#');
            else if (x == snakeX && y == snakeY)
                drawCharXY(x, y, 'O');
            else if (x == fruitX && y == fruitY)
                drawCharXY(x, y, 'F');
            else 
                drawCharXY(x, y, ' ');
        }
    }

    printf("\x1b[22;1HScore: %i    ", score);

    gfxFlushBuffers();
    gfxSwapBuffers();
    gspWaitForVBlank();
}

void Input()
{
    hidScanInput();
    u32 kDown = hidKeysDown();

    if ((kDown & KEY_DLEFT)  && dir != RIGHT) dir = LEFT;
    if ((kDown & KEY_DRIGHT) && dir != LEFT)  dir = RIGHT;
    if ((kDown & KEY_DUP)    && dir != DOWN)  dir = UP;
    if ((kDown & KEY_DDOWN)  && dir != UP)    dir = DOWN;

    if (kDown & KEY_START) gameOver = true;
}

void Logic()
{
    if (speedFormula()) {
        switch(dir)
        {
            case LEFT:
                snakeX--;
                break;
            case RIGHT:
                snakeX++;
                break;
            case UP:
                snakeY--;
                break;
            case DOWN:
                snakeY++;
                break;
            default: 
                break;
        }
    }

    if ((snakeX == fruitX) && (snakeY == fruitY)) {
        score++; nTail++;
        seedFruit();
    }

    if (snakeX >= width || snakeX <= 0 || snakeY >= height || snakeY <= 0)
    {
        gameOver = true;
    }
}

int main() {
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, NULL);

    while (aptMainLoop()) 
    {
        Setup();

        while(!gameOver)
        {
            frame++;
            Input();
            Draw();
            Logic();
        }

        hidScanInput();
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START) break;
    }

    gfxExit();
    return 0;
}
