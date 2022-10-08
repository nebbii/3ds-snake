#include <3ds.h>
//#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

bool gameOver;
const int width = 20;
const int height = 20;
int snakeX, snakeY, fruitX, fruitY, score;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;

void drawCharXY(int x, int y, char s) {
    printf("\x1b[%i;%iH%c", y+1, x+1, s);
}

void seedFruit() {
    fruitX = 1 + rand() % (width-1);
    fruitY = 1 + rand() % (height-1);
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
    printf("\x1b[22;1HScore: %i    ", score);
    printf("\x1b[23;1HFruit posX: %i   ", fruitX);
    printf("\x1b[24;1HFruit posY: %i   ", fruitY);

    // playfield
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

    gfxFlushBuffers();
    gfxSwapBuffers();
    gspWaitForVBlank();
}

void Input()
{
    hidScanInput();
    u32 kDown = hidKeysDown();

    if (kDown & KEY_DLEFT) dir = LEFT;
    if (kDown & KEY_DRIGHT) dir = RIGHT;
    if (kDown & KEY_DUP) dir = UP;
    if (kDown & KEY_DDOWN) dir = DOWN;
    if (kDown & KEY_START) gameOver = true;
}

void Logic()
{
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

    if ((snakeX == fruitX) && (snakeY == fruitY)) {
        score++;
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
            Input();
            Draw();
            Logic();
            svcSleepThread(50000 * 1000);
        }

        hidScanInput();
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START) break;
    }

    gfxExit();
    return 0;
}
