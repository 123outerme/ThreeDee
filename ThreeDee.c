#include "ThreeDee.h"

int initSDL(char* windowName, int screenWidth, int screenHeight, int tileSize, char* textureFilePath, char* fontFilePath, int fontSize)
{
    SCREEN_WIDTH = screenWidth;
    SCREEN_HEIGHT = screenHeight;
    TILE_SIZE = tileSize;
    int done = 0;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    else
    {
        if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
        {
            printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
            return -1;
        }
        //Initialize SDL_ttf
        if(TTF_Init() == -1)
        {
            printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
            return -2;
        }
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
        {
            printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", SDL_GetError());
            return -3;
        }
        else
        //Mix_Init(MIX_INIT_OGG);  //deprecated?
        soundVolume = MIX_MAX_VOLUME;
        Mix_AllocateChannels(32);
        Mix_Volume(-1, soundVolume);  //sets all channels to the sound level soundVolume
        musicVolume = MIX_MAX_VOLUME;
        Mix_VolumeMusic(musicVolume);
        mainScreen = NULL;
        globalTexture = NULL;
        mainRenderer = NULL;
        mainFont = NULL;
        hasText = true;
        hasTiles = true;
        mainWindow = SDL_CreateWindow(windowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if (!mainWindow)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            return 2;
        }
        else
        {
            mainScreen = SDL_GetWindowSurface(mainWindow);
            mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);
            if(!mainRenderer)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                return 3;
            }
            else
            {
                /*SDL_Surface* iconSurface = SDL_LoadBMP("icon.png");
                SDL_SetWindowIcon(mainWindow, iconSurface);
                SDL_FreeSurface(iconSurface);*/
                SDL_SetRenderDrawBlendMode(mainRenderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(mainRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderSetLogicalSize(mainRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
                SDL_RenderClear(mainRenderer);
                loadTTFont(fontFilePath, &mainFont, fontSize);
                //loadTTFont(FONT_FILE_NAME, &smallFont, 20);
                if (!mainFont)
                    hasText = false;

                loadIMG(textureFilePath, &globalTexture);
                if (!globalTexture)
                    hasTiles = false;
                else
                {
                    srand((unsigned int) time(NULL));
                }
            }
        }
    }
    return done;
}

bool loadIMG(char* imgPath, SDL_Texture** dest)
{
    SDL_Surface* surf = IMG_Load(imgPath);
    if (!surf)
    {
	printf("Unable to load image for %s! SDL_Error: %s\n", imgPath, SDL_GetError());
        return false;
    }
    SDL_SetColorKey(surf, 1, SDL_MapRGB(surf->format, 255, 28, 198));
    *dest = SDL_CreateTextureFromSurface(mainRenderer, surf);
    if (!(*dest))
    {
        printf("Unable to create texture for %s! SDL_Error: %s\n", imgPath, SDL_GetError());
        return false;
    }
    SDL_FreeSurface(surf);
    return true;
}

bool loadTTFont(char* filePath, TTF_Font** dest, int sizeInPts)
{
    *dest = TTF_OpenFont(filePath, sizeInPts);
    if (!*dest)
    {
        printf("Font could not be loaded! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

int* loadTextTexture(char* text, SDL_Texture** dest, int maxW, SDL_Color color, bool isBlended)
{
    static int wh[] = {0, 0};
    SDL_Surface* txtSurface = NULL;
    if (isBlended)
        txtSurface = TTF_RenderText_Blended_Wrapped(mainFont, text, color, maxW);
//    else
//        txtSurface = TTF_RenderText(smallFont, text, color, ((SDL_Color) {181, 182, 173}));
    *dest = SDL_CreateTextureFromSurface(mainRenderer, txtSurface);
    if (!*dest)
    {
        printf("Text texture could not be loaded! SDL Error: %s\n", SDL_GetError());
    }
    else
    {
        wh[0] = txtSurface->w;
        wh[1] = txtSurface->h;
    }
    SDL_FreeSurface(txtSurface);
    return wh;
}

void initPt(TDPoint* ptPtr, double x, double y, double z)
{
    ptPtr->x = x;
    ptPtr->y = y;
    ptPtr->z = z;
}

void initTri(TDTri* triPtr, TDPoint ptArr[])
{
    for(int i = 0; i < 3; i++)
        initPt(&(triPtr->pts[i]), ptArr[i].x, ptArr[i].y, ptArr[i].z);
}

void initCam(TDCam* camPtr, TDPoint point, int frustrum, int vanishingPt, int angle, double zoom)
{
    camPtr->point = point;
    camPtr->angle = angle;
    camPtr->frustrum = frustrum;
    camPtr->vanishingPt = vanishingPt;
    camPtr->zoom = zoom;
}

void rotatePoint(TDPoint* ptPtr, TDPoint aroundPt, double degrees)
{
    ptPtr->x = aroundPt.x + ((ptPtr->x - aroundPt.x) * cos(degrees * PI / 180.0) - (aroundPt.z - ptPtr->z) * sin(degrees * PI / 180.0));
    //ptPtr->y = aroundPt.z + ((ptPtr->y - aroundPt.z) * sin(degrees * PI / 180.0) - (aroundPt.z - ptPtr->y) * cos(degrees * PI / 180.0));
}

SDL_Point draw3DPoint(TDPoint point, TDCam cam, SDL_Color color)
{
    TDPoint newPt = {.x = (cam.zoom * TILE_SIZE) * (point.x - cam.point.x) * (point.z - cam.vanishingPt) / ((cam.point.z * TILE_SIZE) - cam.vanishingPt),
    .y = (cam.zoom * TILE_SIZE) * (point.y - cam.point.y) * (point.z - cam.vanishingPt) / ((cam.point.z * TILE_SIZE) - cam.vanishingPt),
    .z = -1};
    rotatePoint(&newPt, cam.point, cam.angle);
    newPt.x -= cam.point.x * TILE_SIZE;
    newPt.y -= cam.point.y * TILE_SIZE;
    SDL_SetRenderDrawColor(mainRenderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawPoint(mainRenderer, newPt.x, newPt.y);
    return (SDL_Point) {.x = newPt.x, .y = newPt.y};
}

void draw3DLine(TDPoint pt1, TDPoint pt2, TDCam cam, SDL_Color color)
{
    SDL_Point newPoint1 = draw3DPoint(pt1, cam, (SDL_Color) {0x00, 0x00, 0x00, 0xFF});
    SDL_Point newPoint2 = draw3DPoint(pt2, cam, (SDL_Color) {0x00, 0x00, 0x00, 0xFF});
    SDL_SetRenderDrawColor(mainRenderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(mainRenderer, newPoint1.x, newPoint1.y, newPoint2.x, newPoint2.y);
}

void drawTri(TDTri tri, TDCam cam, SDL_Color color)
{
    for(int i = 0; i < 3; i++)
        draw3DLine(tri.pts[i], tri.pts[(i + 1) % 3], cam, color);
}

void drawText(char* input, int x, int y, int maxW, int maxH, SDL_Color color, bool render)
{
    if (hasText)
    {
        SDL_Texture* txtTexture = NULL;
        int* wh = loadTextTexture(input, &txtTexture, maxW, color, true);
        SDL_RenderCopy(mainRenderer, txtTexture, &((SDL_Rect){.w = *wh > maxW ? maxW : *wh, .h = *(wh + 1) > maxH ? maxH : *(wh + 1)}),
                                                 &((SDL_Rect){.x =  x, .y = y, .w = *wh > maxW ? maxW : *wh, .h = *(wh + 1) > maxH ? maxH : *(wh + 1)}));
        if (render)
            SDL_RenderPresent(mainRenderer);
        SDL_DestroyTexture(txtTexture);
    }
}

SDL_Keycode getKey()
{
    SDL_Event e;
    SDL_Keycode keycode = 0;
    while(SDL_PollEvent(&e) != 0)
    {
        if(e.type == SDL_QUIT)
            keycode = -1;
        else
            if(e.type == SDL_KEYDOWN)
                keycode = e.key.keysym.sym;
    }
    return keycode;
}

SDL_Keycode waitForKey()
{
    SDL_Keycode keycode = SDLK_ESCAPE;
    while(!(keycode = getKey()))
    {}
    return keycode;
}

int closeSDL()
{
    SCREEN_WIDTH = 0;
    SCREEN_HEIGHT = 0;
    TILE_SIZE = 0;
    return 0;
}
