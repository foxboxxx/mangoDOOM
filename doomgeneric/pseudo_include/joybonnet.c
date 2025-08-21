// Daniel's Joy Bonnet code

#include "joybonnet.h"
#include "gpio.h"
#include "gpio_extra.h"
#include "i2c.h"
#include "timer.h"
#include "assert.h"

#define ARRAY_LEN(x) (sizeof(x) / sizeof(*x))

static void buttons_init();
static void joystick_init();

void joybonnet_init() {
    gpio_init();
    buttons_init();
    joystick_init();
}

// ~~~~~~~~~~ BUTTONS ~~~~~~~~~~

typedef struct {
    button_t me;
    int gpio;
} button_info_t;

static const button_info_t BUTTONS[] = {
    {BUTTON_A, GPIO_PC0},
    {BUTTON_B, GPIO_PB11},
    {BUTTON_X, GPIO_PB2},
    {BUTTON_Y, GPIO_PB12},
    {BUTTON_SELECT, GPIO_PB3},
    {BUTTON_START, GPIO_PD17},
    {BUTTON_P1, GPIO_PB1},
    {BUTTON_P2, GPIO_PB0},
};

static void buttons_init() {
    for (int i = 0; i < ARRAY_LEN(BUTTONS); i++) {
        gpio_set_input(BUTTONS[i].gpio);
        gpio_set_pullup(BUTTONS[i].gpio);
    }
}

bool button_read(button_t b) {
    return gpio_read(BUTTONS[b].gpio) == 0;
}

// ~~~~~~~~~~ JOYSTICK ~~~~~~~~~~

#define ADS1015_ADDR        0x48
#define ADS1015_REG_CONVERT 0x00
#define ADS1015_REG_CONFIG  0x01

#define CONFIG_SINGLE_SHOT  0b1000000000000000
#define CONFIG_CHAN_0       0b0100000000000000
#define CONFIG_CHAN_1       0b0101000000000000
#define CONFIG_FSR_4096     0b0000001000000000
#define CONFIG_1600_SPS     0b0000000010000000
#define CONFIG_DISABLE_COMP 0b0000000000000011

#define JOYSTICK_VERTICAL   0
#define JOYSTICK_HORIZONTAL 1

static i2c_device_t *joystick_dev;
static int horiz_base = 0;
static int vert_base = 0;

static void joystick_init() {
    i2c_init(I2C_100KHZ);
    joystick_dev = i2c_new(ADS1015_ADDR, I2C_REG_16BIT);
    horiz_base = joystick_read(JOYSTICK_HORIZ);
    vert_base = joystick_read(JOYSTICK_VERT);
}

int joystick_read(joystick_dir_t dir) {
    uint16_t CFG_WORD = CONFIG_SINGLE_SHOT
        | (dir == JOYSTICK_VERTICAL ? CONFIG_CHAN_0 : CONFIG_CHAN_1)
        | CONFIG_FSR_4096
        | CONFIG_1600_SPS
        | CONFIG_DISABLE_COMP;

    uint8_t config_data[3];
    config_data[0] = ADS1015_REG_CONFIG;
    config_data[1] = CFG_WORD >> 8;
    config_data[2] = CFG_WORD & 0xFF;

    // Write configuration
    if (!i2c_block_write(joystick_dev, config_data, 3)) {
        // error("Joystick communication failed.")
    }

    // Wait for conversion to complete (at 1600SPS, ~625us)
    timer_delay_us(1000);

    // Set pointer to conversion register
    uint8_t pointer_reg = ADS1015_REG_CONVERT;
    if (!i2c_block_write(joystick_dev, &pointer_reg, 1)) {
        // error("Joystick communication failed.")
    }

    // Read 2 bytes from conversion register
    uint8_t read_buf[2];
    if (!i2c_block_read(joystick_dev, read_buf, 2)) {
        // error("Joystick communication failed.")
    }

    // ADS1015 gives 12-bit result left-aligned in 16 bits
    int raw = (read_buf[0] << 8) | read_buf[1];
    raw = raw >> 4;  // Right-align the 12-bit result

    return raw - (dir == JOYSTICK_VERTICAL ? vert_base : horiz_base);
}