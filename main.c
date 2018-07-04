#include "ThreeDee.h"

int main(int argc, char* argv[])
{
    initSDL("ThreeDee Engine", 20 * 32, 15 * 32, 32, "./cb.bmp", "./Px437_ITT_BIOS_X.ttf", 20);
    {
        TDPoint camPt;
        initPt(&camPt, 0, 0, -10);
        initCam(&mainCamera, camPt, 10, -10, 0, 0, 1.0);
    }
    TDPoint triPts[3][3];
    int triX[3][3] = {{0, 5, 0}, {0, 5, 0}, {0, 5, 0}};
    int triY[3][3] = {{0, 5, 5}, {0, 5, 5}, {0, 5, 5}};
    int triZ[3][3] = {{0, 0, 0}, {5, 5, 5}, {-5, -5, -5}};
    for(int x = 0; x < 3; x++)
    {
        for(int i = 0; i < 3; i++)
        {
            initPt(&(triPts[x][i]), triX[x][i], triY[x][i], triZ[x][i]);
        }
    }
    TDTri newTris[3];
    for(int i = 0; i < 3; i++)
        initTri(&newTris[i], triPts[i]);
    bool quit = false;
    while(!quit)
    {
        SDL_SetRenderDrawColor(mainRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        clearScreen();
        SDL_Keycode key = getKey();
        if (key == SDLK_a || key == SDLK_d)
            mainCamera.point.x += ((key == SDLK_d) - (key == SDLK_a));

        if (key == SDLK_w || key == SDLK_s)
            mainCamera.point.y += ((key == SDLK_w) - (key == SDLK_s));

        if (key == SDLK_UP || key == SDLK_DOWN)
        {
            mainCamera.point.z += ((key == SDLK_DOWN) - (key == SDLK_UP));
            //mainCamera.zoom += .1 * ((key == SDLK_UP) - (key == SDLK_DOWN));
        }
        if (key == SDLK_LEFT || key == SDLK_RIGHT)
        {
            mainCamera.angle = (mainCamera.angle + 1 * (key == SDLK_LEFT ? -1 : 1)) % 360;
            /*for(int i = 0; i < 3; i++)
                rotatePoint(&(newTri.pts[i]), mainCamera.point, mainCamera.angle);*/
        }
        if (key == SDLK_q || key == SDLK_e)
        {
            mainCamera.skew = (mainCamera.skew + 1 * (key == SDLK_q ? -1 : 1)) % 360;
        }
        if (key == SDLK_ESCAPE || key == -1)
            quit = true;
        char debugText[8][9];
        snprintf(debugText[0], 7, "x %f", newTris[0].pts[0].x);
        snprintf(debugText[1], 7, "y %f", newTris[0].pts[0].y);
        snprintf(debugText[2], 7, "z %f", newTris[0].pts[0].z);
        snprintf(debugText[3], 8, "cx %f", mainCamera.point.x);
        snprintf(debugText[4], 8, "cy %f", mainCamera.point.y);
        snprintf(debugText[5], 8, "cz %f", mainCamera.point.z);
        snprintf(debugText[6], 8, "cr %d", mainCamera.angle);
        snprintf(debugText[7], 8, "cs %d", mainCamera.skew);
        for(int i = 0; i < 8; i++)
        {
            drawText(debugText[i], SCREEN_WIDTH - (strlen(debugText[i]) * TILE_SIZE), i * TILE_SIZE, SCREEN_WIDTH, SCREEN_HEIGHT, (SDL_Color) {0x00, 0x00, 0x00, 0xFF}, false);
        }
        for(int i = 0; i < 3; i++)
            drawTri(newTris[i], mainCamera, (SDL_Color) {.r = 0x00, .g = 0x00, .b = 0xFF, .a = 0xFF});
        updateScreen();
    }
    closeSDL();
    return 0;
}
