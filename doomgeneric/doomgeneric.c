#include <stdio.h>
#include "printf.h"

#include "m_argv.h"

#include "doomgeneric.h"

pixel_t* DG_ScreenBuffer = NULL;

// Initial screen states - assume SPI LCD and switch only if HDMI is found
int screen_mode = RGB565;
int DOOMGENERIC_RESX = 320;
int DOOMGENERIC_RESY = DOOMGENERIC_RESY_LCD;

void M_FindResponseFile(void);
void D_DoomMain (void);

void doomgeneric_Create(int argc, char **argv)
{
	// save arguments
    myargc = argc;
    myargv = argv;

	// no-op
	// M_FindResponseFile();

	DG_ScreenBuffer = malloc(DOOMGENERIC_RESX * DOOMGENERIC_RESY * 4);
	printf("Created buffer @%x!", DG_ScreenBuffer);

	DG_Init();

	D_DoomMain ();
}

