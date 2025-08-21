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
#include "i2s.h"
#include "printf.h"
#include "spi.h"
#include "string.h"
#include "timer.h"
#include "uart.h"
#include "doom_theme.h"
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
    // RGBA8888 ON HDMI
    if (screen_mode == RGBA8888) {
        de_set_active_framebuffer(DG_ScreenBuffer);
    }

    // RGB565 ON SPI_LCD
    else if (screen_mode == RGB565) {
        uint16_t *flex = (uint16_t*)DG_ScreenBuffer;
        set_window(0, 20, DOOMGENERIC_RESX, DOOMGENERIC_RESY - 20);
        const static uint8_t mad = 0b01100000;
        send_cmd(CMD_MAD_CTRL); 
        send_data(&mad, 1);
        refresh_screen(75);
        send_cmd(CMD_RAM_WRITE);
        send_data((void *)flex, DOOMGENERIC_RESX * DOOMGENERIC_RESY * 2 - (DOOMGENERIC_RESX * 80));
    }

    printf("SCREENMODE:%d\n", screen_mode);

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

static bool hdmi_plug_is_connected(void) {
    const uint8_t HOT_PLUG_DETECT = 1 << 1;
    uint8_t *phy_stat = (void *)(0x5500000 + 0x3004);
    return (*phy_stat & HOT_PLUG_DETECT) != 0;
}

int main(void) {
    enable_fp();
    uart_init();
    // uart_reinit_custom(0, 115200, GPIO_PF2, GPIO_PF4, GPIO_FN_ALT3); // custom re-init for sdcart uart
    timer_init();

    dma_init();
    spi_init();
    i2s_init(gwav->frames_per_sec);
    i2s_frame_type_t ftype = (gwav->samples_per_frame == 1) ? I2S_MONO : I2S_STEREO;
    joybonnet_init();


    // if (SCREEN_MODE == RGB565) {
    ili9341_init(DOOMGENERIC_RESX, DOOMGENERIC_RESY_LCD, 10);
    // }

    // else if (SCREEN_MODE == RGBA8888) {
    hdmi_resolution_id_t id = hdmi_best_match(DOOMGENERIC_RESX, DOOMGENERIC_RESY_HDMI);
    hdmi_init(id);
    de_init(DOOMGENERIC_RESX, DOOMGENERIC_RESY_HDMI, hdmi_get_screen_width(), hdmi_get_screen_height());
    // }

    doomgeneric_Create(0, NULL);
    i2s_stream_playback_nb(gwav->data, gwav->n_data, ftype);

    for (int i = 0;; i++) {
        if (hdmi_plug_is_connected() && screen_mode != RGBA8888) {
            screen_mode = RGBA8888;
            DOOMGENERIC_RESY = DOOMGENERIC_RESY_HDMI;
            make_black();
        } 
        else if (!hdmi_plug_is_connected() && screen_mode == RGBA8888) {
            make_black();
            screen_mode = RGB565;
        }
        else {
            DOOMGENERIC_RESY = DOOMGENERIC_RESY_LCD;
        }
        doomgeneric_Tick();
        if (i2s_transfer_completed()) i2s_stream_playback_nb(gwav->data, gwav->n_data, ftype);
    }

    return 0;
}