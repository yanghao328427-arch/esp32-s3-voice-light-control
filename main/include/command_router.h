#pragma once

#include <stdint.h>

typedef enum {
    VOICE_CMD_OPEN_LIGHT = 1,
    VOICE_CMD_CLOSE_LIGHT,
    VOICE_CMD_INCREASE_BRIGHTNESS,
    VOICE_CMD_DECREASE_BRIGHTNESS,
    VOICE_CMD_DELAY_OFF_1H,
    VOICE_CMD_DELAY_OFF_2H,
    VOICE_CMD_DELAY_OFF_3H,
} voice_command_id_t;

void command_router_execute(voice_command_id_t command, uint32_t delay_hours);
