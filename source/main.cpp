#include <3ds.h>
//#include <iostream>
#include <stdio.h>

using namespace std;
bool gameOver;

void Setup()
{
    gameOver = false;

}

void Draw()
{

}

void Input()
{


}

void Logic()
{

}

int main() {
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, NULL);

    while (aptMainLoop()) 
    {
        hidScanInput();
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START) break;
        // -------------------------

        Setup();
        while(!gameOver)
        {
            Draw();
            Input();
            Logic();
        }

        // -------------------------
        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    gfxExit();
    return 0;
}
