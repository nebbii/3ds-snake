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
int snakeX, snakeY, fruitX, fruitY, score, hiScore, frame;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir, prevDir;
int nTail;

struct TailXY {
    int x, y;
};

std::vector<TailXY> tailPieces;

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

void setup()
{
    gameOver = false;
    dir = STOP;
    snakeX = width / 2;
    snakeY = height / 2;

    tailPieces.clear();
    nTail = 0;

    seedFruit();

    score = 0;
}

void draw()
{
    if (speedFormula()) {
        for(int x = 0; x <= width; x++) {
            for(int i = 1; i < nTail; i++) {
                drawCharXY(tailPieces[i].x, tailPieces[i].y, 'o');
            }

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
    }

    printf("\x1b[22;1HScore: %i    ", score);
    printf("\x1b[24;1HHiScore: %i    ", hiScore);

    gfxFlushBuffers();
    gfxSwapBuffers();
    gspWaitForVBlank();
}

void input()
{
    hidScanInput();
    u32 kDown = hidKeysDown();

    if ((kDown & KEY_DLEFT) && prevDir != RIGHT) dir = LEFT;
    if ((kDown & KEY_DRIGHT) && prevDir != LEFT) dir = RIGHT;
    if ((kDown & KEY_DUP) && prevDir != DOWN)    dir = UP;
    if ((kDown & KEY_DDOWN) && prevDir != UP)    dir = DOWN;

    //if (kDown & KEY_B) gameOver = true;
}

void logic()
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
        prevDir = dir;

        tailPieces.insert(tailPieces.begin(), {snakeX, snakeY});

        if (tailPieces.size() > nTail) tailPieces.pop_back();
    }

    if ((snakeX == fruitX) && (snakeY == fruitY)) {
        score++; nTail++;
        if (score > hiScore) hiScore = score;
        seedFruit();
    }

    if (snakeX >= width || snakeX <= 0 || snakeY >= height || snakeY <= 0)
    {
        gameOver = true;
    }

    for(int i = 1; i < nTail; i++) {
        if((snakeX == tailPieces[i].x) && (snakeY == tailPieces[i].y))
            gameOver = true;
    }
}

int main() {
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, NULL);
    draw();

    while (aptMainLoop()) 
    {
        hidScanInput();
        u32 kDown = hidKeysDown();

        printf("\x1b[6;8HWelcome");
        printf("\x1b[7;10Hto");
        printf("\x1b[8;6HNebi Snake!");

        printf("\x1b[12;3HPress A to Start");

        printf("\x1b[15;3HPress B to close");

        if (kDown & KEY_A) setup();

        while(!gameOver)
        {
            frame++;
            input();
            draw();
            logic();
        }

        if (kDown & KEY_B) break;
    }

    gfxExit();
    return 0;
}
