#include "mqtt_bridge.h"

#include <stdio.h>

#include "esp_log.h"
#include "mqtt_client.h"

#include "light_controller.h"

static const char *TAG = "mqtt";

#if CONFIG_LIGHT_MQTT_ENABLED
static esp_mqtt_client_handle_t s_client;

static void mqtt_event_handler(void *arg, esp_event_base_t base, int32_t event_id, void *event_data)
{
    (void)arg;
    (void)base;
    (void)event_data;
    if (event_id == MQTT_EVENT_CONNECTED) {
        ESP_LOGI(TAG, "connected");
    }
}
#endif

void mqtt_bridge_start(void)
{
#if CONFIG_LIGHT_MQTT_ENABLED
    const esp_mqtt_client_config_t config = {
        .broker.address.uri = CONFIG_LIGHT_MQTT_BROKER_URI,
    };
    s_client = esp_mqtt_client_init(&config);
    ESP_ERROR_CHECK(esp_mqtt_client_register_event(s_client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL));
    ESP_ERROR_CHECK(esp_mqtt_client_start(s_client));
#else
    ESP_LOGI(TAG, "MQTT disabled; enable it in menuconfig after entering the private server URI");
#endif
}

void mqtt_bridge_publish_light_state(void)
{
#if CONFIG_LIGHT_MQTT_ENABLED
    if (s_client == NULL) {
        return;
    }
    light_state_t state = light_controller_get_state();
    char payload[64];
    int length = snprintf(payload, sizeof(payload), "{\"on\":%s,\"brightness\":%u}",
                          state.is_on ? "true" : "false", state.brightness_percent);
    esp_mqtt_client_publish(s_client, CONFIG_LIGHT_MQTT_TOPIC, payload, length, 1, false);
#endif
}
