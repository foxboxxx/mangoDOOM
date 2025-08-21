#pragma once

#include "dma.h"
#include "gpio.h"
#include "printf.h"
#include "spi.h"
#include "spi_nb.h"
#include "timer.h"
#include "uart.h"

/**
 * colors in RGB565 format (big-endian)
 */
#define WHITE       0xFFFF
#define BLACK       0x0000
#define BLUE        0x1F00
#define RED         0x00F8
#define GREEN       0xE007
#define MAGENTA     0x1FF8
#define CYAN        0xFF7F
#define YELLOW      0xE0FF

enum {
    CMD_SW_RESET        = 0x01,
    CMD_MAD_CTRL        = 0x36,
    CMD_SLEEP_IN        = 0x10,
    CMD_SLEEP_OUT       = 0x11,
    CMD_INVERT_OFF      = 0x20,
    CMD_INVERT_ON       = 0x21,
    CMD_DISPLAY_OFF     = 0x28,
    CMD_DISPLAY_ON      = 0x29,
    CMD_RANGE_X_SET     = 0x2A,
    CMD_RANGE_Y_SET     = 0x2B,
    CMD_RAM_WRITE       = 0x2C,
    CMD_COLOR_MODE_SET  = 0x3A,
    COLOR_MODE_RGB565   = 0x55,
};

// Function declarations
void refresh_screen(int mhz);
void send_cmd(uint8_t cmd);
void send_data(uint8_t *data, uint32_t n);
void send_data_nb(uint8_t *data, uint32_t n);
void set_window(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void make_black(void);
void ili9341_init(int width, int height, int mhz);