// quick & dirty test to fill pixels on LCD screen via SPI
// This is for 240x320 LCD using ili9341 driver
// juliez summer 2025

// daniel spi lcd code
#include "spi_lcd.h"
#include "malloc.h"
#include "string.h"

static struct {
    int width, height;
    spi_device_t *spi_dev;
    gpio_id_t gpio_cs, gpio_dc;
} module = {
    .spi_dev = NULL,
    .gpio_cs = GPIO_PD10,
    .gpio_dc = GPIO_PC1, // drive high/low to switch send command vs data
};

void refresh_screen(int mhz) {
    module.spi_dev = spi_new(module.gpio_cs, SPI_MODE_3, mhz*1000*1000);
}

void send_cmd(uint8_t cmd) {
    gpio_write(module.gpio_dc, 0);  // DC low = command
    spi_write_v(module.spi_dev, cmd);
}

void send_data(uint8_t *data, uint32_t n) {
    gpio_write(module.gpio_dc, 1);  // DC high = data
    spi_write(module.spi_dev, data, n);
}

void send_data_nb(uint8_t *data, uint32_t n) {
    gpio_write(module.gpio_dc, 1);  // DC high = data
    spi_write_nb(module.spi_dev, data, n);
}

void set_window(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    uint16_t x_end = x + w - 1, y_end = y + h - 1;

    send_cmd(CMD_RANGE_X_SET);
    uint8_t cdata[] = {x >> 8, x, x_end >> 8, x_end};
    send_data(cdata, sizeof(cdata));

    send_cmd(CMD_RANGE_Y_SET);
    uint8_t rdata[] = {y >> 8, y, y_end >> 8, y_end};
    send_data(rdata, sizeof(rdata));
}

void make_black(void) {
    static uint8_t *fb = NULL;
    if (fb == NULL) {
        fb = malloc(module.width * module.height * 2);
        memset(fb, 0x0, module.width * module.height * 2);
    }
    set_window(0, 0, module.width, module.height);
    send_cmd(CMD_RAM_WRITE);
    send_data((void *)fb, module.width * module.height * 2);
}

void ili9341_init(int width, int height, int mhz) {
    module.width = width;
    module.height = height;
    gpio_set_output(module.gpio_dc); // signal to switch between cmd/data mode
    module.spi_dev = spi_new(module.gpio_cs, SPI_MODE_3, mhz*1000*1000);
    //send_cmd(CMD_SW_RESET); // not needed, tie reset pin to 3.3v header will hardware reset with Pi
    send_cmd(CMD_COLOR_MODE_SET);
    send_data((uint8_t []){COLOR_MODE_RGB565}, 1);
    // set_window(0, 0, module.width, module.height); // not needed?
    send_cmd(CMD_SLEEP_OUT);
    send_cmd(CMD_DISPLAY_ON);
    // set screen to black
    make_black();
}