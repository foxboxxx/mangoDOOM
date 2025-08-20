#include "doomgeneric.h"
#include "doomkeys.h"
#include "m_argv.h"

#include "mango.h"
#include "de.h"
#include "gl.h"
#include "hdmi.h"
#include "i2c.h"
#include "dma.h"
#include "spi.h"
// #include "i2s.h"
#include "printf.h"
#include "spi.h"
#include "string.h"
#include "timer.h"
#include "uart.h"
void uart_reinit_custom(int, int, gpio_id_t, gpio_id_t, unsigned int);

#include "spi_lcd.h"
#include "joybonnet.h"

#define J_THRESH 500

extern void enable_fp(void);

static unsigned char getDoomKey(void) {
    int jx = joystick_read(JOYSTICK_HORIZ);
    int jy = joystick_read(JOYSTICK_VERT);
    if (jx > J_THRESH)  { return KEY_RIGHTARROW; }
    else if (jx < -J_THRESH) { return KEY_LEFTARROW; }
    else if (jy > J_THRESH)  { return KEY_DOWNARROW; }
    else if (jy < -J_THRESH) { return KEY_UPARROW; }
    else if (button_read(BUTTON_A)) { return KEY_FIRE; }
    else if (button_read(BUTTON_START)) { return KEY_ESCAPE; }
    else if (button_read(BUTTON_X)) { return KEY_USE; }
    else if (button_read(BUTTON_SELECT)) { return KEY_ENTER; }
    return 0;
}

void DG_Init() { 
    return; 
}

void DG_DrawFrame() {
    // direct set (fastest)
    de_set_active_framebuffer(DG_ScreenBuffer);

    uint16_t *flex = (uint16_t*)DG_ScreenBuffer;

    // uint16_t temp[DOOMGENERIC_RESX * DOOMGENERIC_RESY];
    // for (int i = 0; i < DOOMGENERIC_RESX; i++) {
    //     for (int j = 0; j < DOOMGENERIC_RESY; j++) {
    //         temp[i * DOOMGENERIC_RESY + j] = flex[(DOOMGENERIC_RESY - j - 1) * DOOMGENERIC_RESX + i];
    //     }
    // }
    // time_draw(75);
    set_window(0, 0, DOOMGENERIC_RESX, DOOMGENERIC_RESY);
    const static uint8_t mad = 0b01100000;
    send_cmd(CMD_MAD_CTRL); 
    send_data(&mad, 1);
    refresh_screen(75);
    
    send_cmd(CMD_RAM_WRITE);
    send_data((void *)flex, DOOMGENERIC_RESX * DOOMGENERIC_RESY * 2);
    // while (!spi_transfer_completed(module.spi_dev)) {}
    return;
}

void DG_SleepMs(uint32_t ms) { 
    timer_delay_ms(ms); 
}

uint32_t DG_GetTicksMs() {
    long long mango_ticks = timer_get_ticks();
    uint32_t ms_ticks = (uint32_t)(mango_ticks / (1 * 1000));
    return ms_ticks;
}

int DG_GetKey(int *pressed, unsigned char *doomKey) {
    static char prev_c = 0;
    static int count = 0;
    char c = getDoomKey();

    if (prev_c != 0 && c != prev_c) {
        *doomKey = prev_c;
        prev_c = 0;
        *pressed = 0;
        return 1;
    }
    else if (prev_c == 0 && c > 0) {
        *doomKey = c;
        prev_c = c;
        *pressed = 1;
        return 1;
    }

    return 0;
}

// Not needed
void DG_SetWindowTitle(const char *title) { return; }

int main(void) {
    dma_init();
    spi_init();

    ili9341_init(DOOMGENERIC_RESX, DOOMGENERIC_RESX, 10);

    enable_fp();
    hdmi_resolution_id_t id = hdmi_best_match(DOOMGENERIC_RESX, DOOMGENERIC_RESY);
    hdmi_init(id);
    de_init(DOOMGENERIC_RESX, DOOMGENERIC_RESY, hdmi_get_screen_width(), hdmi_get_screen_height());

    uart_init();
    // uart_reinit_custom(0, 115200, GPIO_PF2, GPIO_PF4, GPIO_FN_ALT3); // custom re-init for sdcart uart

    joybonnet_init();
    timer_init();
    // test_rotate();
    doomgeneric_Create(0, NULL);

    for (int i = 0;; i++) {
        doomgeneric_Tick();
    }

    return 0;
}