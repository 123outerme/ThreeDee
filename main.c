#include "ThreeDee.h"

int main(int argc, char* argv[])
{
    initSDL("ThreeDee Engine", 20 * 32, 15 * 32, 32, "./cb.bmp", "./Px437_ITT_BIOS_X.ttf", 20);
    {
        TDPt camPt;
        initPt(&camPt, 0, 0, 0);
        initCam(&mainCamera, camPt, 10, -10, 0, 1.0);
    }
    TDPt triPts[3];
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
        if (key == SDLK_w || key == SDLK_s)
        {
            mainCamera.point.z += ((key == SDLK_w) - (key == SDLK_s));
        }
        if (key == SDLK_a || key == SDLK_d)
        {
            mainCamera.angle += 10 * (key == SDLK_a ? -1 : 1);
            for(int i = 0; i < 3; i++)
                rotatePoint(&(newTri.pts[i]), mainCamera.point, mainCamera.angle);
        }
        if (key == SDLK_ESCAPE || key == -1)
            quit = true;
            char XText[8];
            char YText[8];
            char ZText[8];
            char camZText[8];
            snprintf(XText, 7, "x %f", newTri.pts[0].x);
            snprintf(YText, 7, "y %f", newTri.pts[0].y);
            snprintf(ZText, 7, "z %f", newTri.pts[0].z);
            snprintf(camZText, 7, "c %f", mainCamera.point.z);
            drawText(XText, SCREEN_WIDTH - (strlen(XText) * TILE_SIZE), 0, SCREEN_WIDTH, SCREEN_HEIGHT, (SDL_Color) {0x00, 0x00, 0x00, 0xFF}, false);
            drawText(YText, SCREEN_WIDTH - (strlen(YText) * TILE_SIZE), TILE_SIZE, SCREEN_WIDTH, SCREEN_HEIGHT, (SDL_Color) {0x00, 0x00, 0x00, 0xFF}, false);
            drawText(ZText, SCREEN_WIDTH - (strlen(ZText) * TILE_SIZE), 2 * TILE_SIZE, SCREEN_WIDTH, SCREEN_HEIGHT, (SDL_Color) {0x00, 0x00, 0x00, 0xFF}, false);
            drawText(camZText, SCREEN_WIDTH - (strlen(ZText) * TILE_SIZE), 3 * TILE_SIZE, SCREEN_WIDTH, SCREEN_HEIGHT, (SDL_Color) {0x00, 0x00, 0x00, 0xFF}, false);
        drawTri(newTri, mainCamera, (SDL_Color) {.r = 0x00, .g = 0x00, .b = 0xFF, .a = 0xFF});
        updateScreen();
    }

    closeSDL();
    return 0;
}
