#include "command_router.h"

#include "esp_log.h"

#include "light_controller.h"
#include "mqtt_bridge.h"

static const char *TAG = "router";
static const int8_t k_brightness_step = 15;

void command_router_execute(voice_command_id_t command, uint32_t delay_hours)
{
    switch (command) {
    case VOICE_CMD_OPEN_LIGHT:
        light_controller_turn_on();
        break;
    case VOICE_CMD_CLOSE_LIGHT:
        light_controller_turn_off();
        break;
    case VOICE_CMD_INCREASE_BRIGHTNESS:
        light_controller_adjust_brightness(k_brightness_step);
        break;
    case VOICE_CMD_DECREASE_BRIGHTNESS:
        light_controller_adjust_brightness(-k_brightness_step);
        break;
    case VOICE_CMD_DELAY_OFF_1H:
    case VOICE_CMD_DELAY_OFF_2H:
    case VOICE_CMD_DELAY_OFF_3H:
        light_controller_schedule_off(delay_hours);
        break;
    default:
        ESP_LOGW(TAG, "unmapped command=%d", command);
        return;
    }
    mqtt_bridge_publish_light_state();
}
