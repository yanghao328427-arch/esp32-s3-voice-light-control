#include "esp_check.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "light_controller.h"
#include "mqtt_bridge.h"
#include "voice_commands.h"

static const char *TAG = "app";

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(light_controller_init());
    mqtt_bridge_start();
    voice_commands_register();
    voice_commands_print_mapping();

    /*
     * Connect the ESP-SR MultiNet callback to voice_commands_handle_recognition().
     * The command phrases and IDs are registered by voice_commands_register().
     * PDM/I2S microphone pin definitions are board-specific and deliberately not guessed.
     */
    ESP_LOGI(TAG, "ready: configure ESP-SR audio frontend and call voice_commands_handle_recognition");
}
