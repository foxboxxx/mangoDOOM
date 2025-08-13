#include "doomkeys.h"
#include "m_argv.h"
#include "doomgeneric.h"

#include "gl.h"
#include "timer.h"
#include "printf.h"
#include "string.h"
#include "i2c.h"
#include "i2s.h"
#include "spi.h"
#include "uart.h"

void DG_Init()
{
    return;
}

void DG_DrawFrame()
{
    return;
}

void DG_SleepMs(uint32_t ms)
{
    timer_delay_ms(ms);
}

uint32_t DG_GetTicksMs()
{
    return timer_get_ticks();
}

int DG_GetKey(int* pressed, unsigned char* doomKey)
{
    return 0;
}

void DG_SetWindowTitle(const char * title)
{
    return;
}

int main(int argc, char **argv)
{
    uart_init();
    doomgeneric_Create(argc, argv);

    for (int i = 0; ; i++)
    {
        doomgeneric_Tick();
        printf("Tick!");
    }
    
    return 0;
}