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

#define KEYQUEUE_SIZE 16

static unsigned short s_KeyQueue[KEYQUEUE_SIZE];
static unsigned int s_KeyQueueWriteIndex = 0;
static unsigned int s_KeyQueueReadIndex = 0;

void DG_Init()
{
    return;
}

void DG_DrawFrame()
{
    // for (int j = 0; j < DOOMGENERIC_RESY; j++) {
    //     for (int i = 0; i < DOOMGENERIC_RESX; i++) {
    //         gl_draw_pixel(i, j, DG_ScreenBuffer[j * DOOMGENERIC_RESX + i]);
    //     }
    // }

    // optimized frame copy
    uint32_t(*framebuffer)[DOOMGENERIC_RESX] = fb_get_draw_buffer();
    // for (int j = 0; j < DOOMGENERIC_RESY; j++) {
        memcpy(framebuffer, DG_ScreenBuffer, DOOMGENERIC_RESX * 4 * DOOMGENERIC_RESY);
    // }

    printf("%s: Frame printed\n", __FUNCTION__);
    gl_swap_buffer();
    handleKeyInput();
    return;
}

void DG_SleepMs(uint32_t ms)
{
    timer_delay_ms(ms);
}

uint32_t DG_GetTicksMs()
{
    long long mango_ticks = timer_get_ticks();
    uint32_t ms_ticks = (uint32_t)(mango_ticks / (TICKS_PER_USEC * 1000));
    return ms_ticks;
}

// TODO: Implement with joystick controls
int DG_GetKey(int *pressed, unsigned char *doomKey) {
    // *pressed = KEY_UPARROW >> 8;
    // *doomKey = KEY_UPARROW & 0xFF;
    // return 1;
    if (s_KeyQueueReadIndex == s_KeyQueueWriteIndex) {
        // key queue is empty
        return 0;
    } else {
        unsigned short keyData = s_KeyQueue[s_KeyQueueReadIndex];
        s_KeyQueueReadIndex++;
        s_KeyQueueReadIndex %= KEYQUEUE_SIZE;

        *pressed = keyData >> 8;
        *doomKey = keyData & 0xFF;

        return 1;
    }
}

// Not needed
void DG_SetWindowTitle(const char * title)
{
    return;
}

int main(int argc, char **argv)
{
    uart_init();
    timer_init();
    gl_init(DOOMGENERIC_RESX, DOOMGENERIC_RESY, GL_DOUBLEBUFFER);
    doomgeneric_Create(argc, argv);

    for (int i = 0; ; i++)
    {
        doomgeneric_Tick();
        printf("Tick!");
    }
    
    return 0;
}