#ifndef DOOM_GENERIC
#define DOOM_GENERIC

#include <stdlib.h>
#include <stdint.h>
#include "screen_controls.h"

#define RGBA8888 0
#define RGB565 1

// #ifndef SCREEN_MODE
// #define SCREEN_MODE RGBA8888
// #endif

// #ifndef DOOMGENERIC_RESX
// #define DOOMGENERIC_RESX 320
// #endif  // DOOMGENERIC_RESX

// #ifndef DOOMGENERIC_RESY
//   #if SCREEN_MODE == RGBA8888
//     #define DOOMGENERIC_RESY 200
//   #elif SCREEN_MODE == RGB565
//     #define DOOMGENERIC_RESY 240
//   #endif
// #endif  // DOOMGENERIC_RESY

#define DOOMGENERIC_RESY_LCD 240
#define DOOMGENERIC_RESY_HDMI 200

#ifdef CMAP256

typedef uint8_t pixel_t;

#else  // CMAP256

typedef uint32_t pixel_t;
typedef pixel_t color_t;

#endif  // CMAP256


extern pixel_t* DG_ScreenBuffer;

#ifdef __cplusplus
extern "C" {
#endif

void doomgeneric_Create(int argc, char **argv);
void doomgeneric_Tick();

void DG_Init();
void DG_DrawFrame();
void DG_SleepMs(uint32_t ms);
uint32_t DG_GetTicksMs();
int DG_GetKey(int* pressed, unsigned char* key);
void DG_SetWindowTitle(const char * title);

#ifdef __cplusplus
}
#endif

#endif //DOOM_GENERIC
