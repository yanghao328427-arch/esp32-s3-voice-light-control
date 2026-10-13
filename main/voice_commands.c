#include "voice_commands.h"

#include "esp_log.h"
#include "esp_mn_speech_commands.h"

#include "command_router.h"

static const char *TAG = "voice";

void voice_commands_register(void)
{
    esp_mn_commands_clear();
    esp_mn_commands_add(VOICE_CMD_OPEN_LIGHT, "打开灯光");
    esp_mn_commands_add(VOICE_CMD_CLOSE_LIGHT, "关闭灯光");
    esp_mn_commands_add(VOICE_CMD_INCREASE_BRIGHTNESS, "增大亮度");
    esp_mn_commands_add(VOICE_CMD_DECREASE_BRIGHTNESS, "减小亮度");
    esp_mn_commands_add(VOICE_CMD_DELAY_OFF_1H, "一小时后自动关灯");
    esp_mn_commands_add(VOICE_CMD_DELAY_OFF_2H, "两小时后自动关灯");
    esp_mn_commands_add(VOICE_CMD_DELAY_OFF_3H, "三小时后自动关灯");
    esp_mn_error_t *errors = esp_mn_commands_update();
    if (errors != NULL) {
        ESP_LOGW(TAG, "one or more command phrases were rejected");
    }
}

void voice_commands_handle_recognition(uint16_t command_id, const char *recognized_text)
{
    ESP_LOGI(TAG, "recognized command=%u text=%s", command_id,
             recognized_text == NULL ? "" : recognized_text);

    switch (command_id) {
    case VOICE_CMD_OPEN_LIGHT:
    case VOICE_CMD_CLOSE_LIGHT:
    case VOICE_CMD_INCREASE_BRIGHTNESS:
    case VOICE_CMD_DECREASE_BRIGHTNESS:
        command_router_execute((voice_command_id_t)command_id, 0);
        break;
    case VOICE_CMD_DELAY_OFF_1H:
        command_router_execute(VOICE_CMD_DELAY_OFF_1H, 1);
        break;
    case VOICE_CMD_DELAY_OFF_2H:
        command_router_execute(VOICE_CMD_DELAY_OFF_2H, 2);
        break;
    case VOICE_CMD_DELAY_OFF_3H:
        command_router_execute(VOICE_CMD_DELAY_OFF_3H, 3);
        break;
    default:
        ESP_LOGW(TAG, "ignored unsupported command id=%u", command_id);
        break;
    }
}

void voice_commands_print_mapping(void)
{
    ESP_LOGI(TAG, "1: 打开灯光; 2: 关闭灯光; 3: 增大亮度; 4: 减小亮度; 5/6/7: 一/两/三小时后自动关灯");
}
