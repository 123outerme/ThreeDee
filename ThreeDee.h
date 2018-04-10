#ifndef THREEDEE_H_INCLUDED
#define THREEDEE_H_INCLUDED

#include <stdio.h>         //This is included for lots of good stuff
#include <stdlib.h>        //This is included for lots of other stuff
#include <string.h>        //Included for the str... and strn... functions
#include <math.h>          //This is included for sin/cos, log10, etc.
#include <time.h>          //This is included for time() as the seed for rand()

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"

#define bool char
#define true 1
#define false 0
#define printBool(bool) bool ? "true" : "false"

#define PI 3.14159265359879

#define clearScreen() SDL_RenderClear(mainRenderer)
#define updateScreen() SDL_RenderPresent(mainRenderer)

typedef struct _TDPt {
    double x;
    double y;
    double z;
} TDPoint;

typedef struct _TDTri {
    TDPoint pts[3];
} TDTri;

typedef struct _TDCam {
    TDPoint point;
    int frustrum;
    int vanishingPt;
    int angle;
    double zoom;
} TDCam;

int initSDL(char* windowName, int screenWidth, int screenHeight, int tileSize, char* textureFilePath, char* fontFilePath, int fontSize);
bool loadIMG(char* imgPath, SDL_Texture** dest);
bool loadTTFont(char* filePath, TTF_Font** dest, int sizeInPts);
int* loadTextTexture(char* text, SDL_Texture** dest, int maxW, SDL_Color color, bool isBlended);
void initPt(TDPoint* ptPtr, double x, double y, double z);
void initTri(TDTri* triPtr, TDPoint ptArr[]);
void initCam(TDCam* camPtr, TDPoint point, int frustrum, int vanishingPt, int angle, double zoom);
void rotatePoint(TDPoint* ptPtr, TDPoint fromPt, double theta);
SDL_Point draw3DPoint(TDPoint point, TDCam cam, SDL_Color color);
void draw3DLine(TDPoint pt1, TDPoint pt2, TDCam cam, SDL_Color color);
void drawTri(TDTri tri, TDCam cam, SDL_Color color);
void drawText(char* input, int x, int y, int maxW, int maxH, SDL_Color color, bool render);
SDL_Keycode getKey();
SDL_Keycode waitForKey();
int closeSDL();

int SCREEN_WIDTH, SCREEN_HEIGHT, TILE_SIZE;
SDL_Window* mainWindow;
SDL_Surface* mainScreen;
SDL_Texture* globalTexture;
SDL_Renderer* mainRenderer;
TTF_Font* mainFont;
TDCam mainCamera;

bool hasText, hasTiles, hasMusic;
int musicVolume, soundVolume;

#endif // THREEDEE_H_INCLUDED
