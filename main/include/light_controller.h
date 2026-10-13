#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "esp_err.h"

typedef struct {
    bool is_on;
    uint8_t brightness_percent;
} light_state_t;

esp_err_t light_controller_init(void);
void light_controller_turn_on(void);
void light_controller_turn_off(void);
void light_controller_adjust_brightness(int8_t delta_percent);
void light_controller_schedule_off(uint32_t delay_hours);
light_state_t light_controller_get_state(void);
