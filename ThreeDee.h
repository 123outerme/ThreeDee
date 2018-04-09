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

#define clearScreen() SDL_RenderClear(mainRenderer)
#define updateScreen() SDL_RenderPresent(mainRenderer)

typedef struct _TDPt {
    double x;
    double y;
    double z;
} TDPt;

typedef struct _TDTri {
    TDPt pts[3];
} TDTri;

typedef struct _TDCam {
    TDPt point;
    int frustrum;
    int vanishingPt;
    int angle;
    double zoom;
} TDCam;

int initSDL(char* windowName, int screenWidth, int screenHeight, int tileSize, char* textureFilePath, char* fontFilePath, int fontSize);
bool loadIMG(char* imgPath, SDL_Texture** dest);
bool loadTTFont(char* filePath, TTF_Font** dest, int sizeInPts);
int* loadTextTexture(char* text, SDL_Texture** dest, int maxW, SDL_Color color, bool isBlended);
void initPt(TDPt* ptPtr, double x, double y, double z);
void initTri(TDTri* triPtr, TDPt ptArr[]);
void initCam(TDCam* camPtr, TDPt point, int frustrum, int vanishingPt, int angle, double zoom);
void rotatePoint(TDPt* ptPtr, TDPt fromPt, double theta);
void draw3DPoint(TDPt point, TDCam cam, SDL_Color color);
void draw3DLine(TDPt pt1, TDPt pt2, TDCam cam, SDL_Color color);
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
