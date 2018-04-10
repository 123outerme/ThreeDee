#include "ThreeDee.h"

int main(int argc, char* argv[])
{
    initSDL("ThreeDee Engine", 20 * 32, 15 * 32, 32, "./cb.bmp", "./Px437_ITT_BIOS_X.ttf", 20);
    {
        TDPoint camPt;
        initPt(&camPt, -15, -10, 0);
        initCam(&mainCamera, camPt, 10, -10, 0, 1.0);
    }
    TDPoint triPts[3];
    int triX[3] = {0, 5, 0};
    int triY[3] = {0, 5, 5};
    int triZ[3] = {0, 0, 0};
    for(int i = 0; i < 3; i++)
    {
        initPt(&(triPts[i]), triX[i], triY[i], triZ[i]);
    }
    TDTri newTri;
    initTri(&newTri, triPts);
    bool quit = false;
    while(!quit)
    {
        SDL_SetRenderDrawColor(mainRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        clearScreen();
        SDL_Keycode key = getKey();
        if (key == SDLK_a || key == SDLK_d)
            mainCamera.point.x += ((key == SDLK_a) - (key == SDLK_d));

        if (key == SDLK_w || key == SDLK_s)
            mainCamera.point.y += ((key == SDLK_w) - (key == SDLK_s));

        if (key == SDLK_UP || key == SDLK_DOWN)
        {
            mainCamera.point.z += ((key == SDLK_UP) - (key == SDLK_DOWN));
        }
        if (key == SDLK_LEFT || key == SDLK_RIGHT)
        {
            mainCamera.angle = (mainCamera.angle + 10 * (key == SDLK_LEFT ? -1 : 1)) % 360;
            /*for(int i = 0; i < 3; i++)
                rotatePoint(&(newTri.pts[i]), mainCamera.point, mainCamera.angle);*/
        }
        if (key == SDLK_ESCAPE || key == -1)
            quit = true;
        char debugText[7][9];
        snprintf(debugText[0], 7, "x %f", newTri.pts[0].x);
        snprintf(debugText[1], 7, "y %f", newTri.pts[0].y);
        snprintf(debugText[2], 7, "z %f", newTri.pts[0].z);
        snprintf(debugText[3], 8, "cx %f", mainCamera.point.x);
        snprintf(debugText[4], 8, "cy %f", mainCamera.point.y);
        snprintf(debugText[5], 8, "cz %f", mainCamera.point.z);
        snprintf(debugText[6], 8, "cr %d", mainCamera.angle);
        for(int i = 0; i < 7; i++)
        {
            drawText(debugText[i], SCREEN_WIDTH - (strlen(debugText[i]) * TILE_SIZE), i * TILE_SIZE, SCREEN_WIDTH, SCREEN_HEIGHT, (SDL_Color) {0x00, 0x00, 0x00, 0xFF}, false);
        }
        drawTri(newTri, mainCamera, (SDL_Color) {.r = 0x00, .g = 0x00, .b = 0xFF, .a = 0xFF});
        updateScreen();
    }
    closeSDL();
    return 0;
}
