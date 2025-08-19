#include "doomgeneric.h"
#include "doomkeys.h"
#include "m_argv.h"

#include "spi_led.c"
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

extern void enable_fp(void);

#include "joybonnet.h"

#define KEYQUEUE_SIZE 16
#define J_THRESH 500

static unsigned short s_KeyQueue[KEYQUEUE_SIZE];
static unsigned int s_KeyQueueWriteIndex = 0;
static unsigned int s_KeyQueueReadIndex = 0;

static unsigned char getDoomKey(void) {
    // if (button_read(BUTTON_A)) return KEY_RIGHTARROW;
    // else if (button_read(BUTTON_Y)) return KEY_LEFTARROW;
    // else if (button_read(BUTTON_X)) return KEY_UPARROW;
    // else if (button_read(BUTTON_B)) return KEY_DOWNARROW;
    // else if (button_read(BUTTON_START)) return KEY_ENTER;
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

static void addKeyToQueue(int pressed, unsigned int input) {

    unsigned short keyData = (pressed << 8) | input;

    s_KeyQueue[s_KeyQueueWriteIndex] = input;
    s_KeyQueueWriteIndex++;
    s_KeyQueueWriteIndex %= KEYQUEUE_SIZE;
}

static void handleKeyInput() {
    static char prev_c = 0;
    char curr = getDoomKey();
    if ((curr > 0) && (curr == prev_c || prev_c == 0)) {
        addKeyToQueue(1, curr);
        prev_c = curr;
    }
    else if (prev_c != 0) {
        addKeyToQueue(0, prev_c);
        prev_c = curr;
    }

}

void DG_Init() { 
    return; 
}

void DG_DrawFrame() {
    // 4th fastest
    // for (int j = 0; j < DOOMGENERIC_RESY; j++) {
    //     for (int i = 0; i < DOOMGENERIC_RESX; i++) {
    //         gl_draw_pixel(i, j, DG_ScreenBuffer[j * DOOMGENERIC_RESX + i]);
    //     }
    // }

    // uint32_t(*framebuffer)[DOOMGENERIC_RESX] = fb_get_draw_buffer();

    // 3rd fastest
    // for (int j = 0; j < DOOMGENERIC_RESY; j++) {

    // 2nd fastest
    // memcpy(framebuffer, DG_ScreenBuffer, DOOMGENERIC_RESX * 4 * DOOMGENERIC_RESY); 
    // framebuffer[j], &DG_ScreenBuffer[j *DOOMGENERIC_RESX], DOOMGENERIC_RESX * 4

    // }
    // gl_swap_buffer();

    // direct set (fastest)
    de_set_active_framebuffer(DG_ScreenBuffer);
    time_draw(75);
    // send_cmd(CMD_RAM_WRITE);
    // send_data_nb((void *)DG_ScreenBuffer, sizeof(DG_ScreenBuffer));


    // static int last = 0, curr = 0;
    // last = curr;
    // curr = DG_GetTicksMs();
    // printf("Draw, %d frames time\n", curr - last);

    // printf("%s: Frame printed\n", __FUNCTION__);
    // handleKeyInput();
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

// TODO: Implement with joystick controls
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

    // if (c > 0 && (count <= 0 || c != prev_c)) {
    //     *doomKey = c;
    //     prev_c = c;  
    //     *pressed = 1;
    //     count = 1;
    //     return 1;
    // }
    // else if (prev_c > 0 && count-- <= 0) {
    //     *pressed = 0;
    //     *doomKey = prev_c;
    //     prev_c = 0;
    //     return 1;
    // }
    return 0;
    // static int x = 0;
    // if (!x) {
    //     *pressed = 1;
    //     *doomKey = KEY_UPARROW;
    //     x = 1;

    //     return 1;
    // }
    // return 0;
}

// Not needed
void DG_SetWindowTitle(const char *title) { return; }

int main(void) {
    dma_init();
    spi_init();

    const int WIDTH = 240, HEIGHT = 320;
    ili9341_init(WIDTH, HEIGHT, 10);

    enable_fp();
    hdmi_resolution_id_t id = hdmi_best_match(DOOMGENERIC_RESX, DOOMGENERIC_RESY);
    hdmi_init(id);
    de_init(DOOMGENERIC_RESX, DOOMGENERIC_RESY, hdmi_get_screen_width(), hdmi_get_screen_height());

    uart_init();
    // uart_reinit_custom(0, 115200, GPIO_PF2, GPIO_PF4, GPIO_FN_ALT3); // custom re-init for sdcart uart

    joybonnet_init();
    timer_init();
    // gl_init(DOOMGENERIC_RESX, DOOMGENERIC_RESY, GL_DOUBLEBUFFER);

    doomgeneric_Create(0, NULL);

    for (int i = 0;; i++) {
        doomgeneric_Tick();
        // printf("Tick!");
    }

    return 0;
}