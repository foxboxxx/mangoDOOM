// Daniel's Joy Bonnet code
#pragma once

#include "stdbool.h"

#define J_THRESH 500

void joybonnet_init();

typedef enum {
    BUTTON_A = 0,
    BUTTON_B,
    BUTTON_X,
    BUTTON_Y,
    BUTTON_SELECT,
    BUTTON_START,
    BUTTON_P1,
    BUTTON_P2,
} button_t;

bool button_read(button_t b);

typedef enum {
    JOYSTICK_VERT,
    JOYSTICK_HORIZ,
} joystick_dir_t;

int joystick_read(joystick_dir_t dir);