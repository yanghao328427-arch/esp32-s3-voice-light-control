#include "light_controller.h"

#include "driver/ledc.h"
#include "esp_log.h"
#include "esp_timer.h"

static const char *TAG = "light";
static const ledc_mode_t k_speed_mode = LEDC_LOW_SPEED_MODE;
static const ledc_channel_t k_channel = LEDC_CHANNEL_0;
static light_state_t s_state = {
    .is_on = false,
    .brightness_percent = CONFIG_LIGHT_DEFAULT_BRIGHTNESS,
};
static esp_timer_handle_t s_off_timer;

static uint32_t percent_to_duty(uint8_t percent)
{
    return ((1U << LEDC_TIMER_13_BIT) - 1U) * percent / 100U;
}

static void apply_output(void)
{
    uint32_t duty = s_state.is_on ? percent_to_duty(s_state.brightness_percent) : 0;
    ESP_ERROR_CHECK(ledc_set_duty(k_speed_mode, k_channel, duty));
    ESP_ERROR_CHECK(ledc_update_duty(k_speed_mode, k_channel));
    ESP_LOGI(TAG, "light=%s brightness=%u%%", s_state.is_on ? "on" : "off", s_state.brightness_percent);
}

static void delayed_off_callback(void *arg)
{
    (void)arg;
    light_controller_turn_off();
}

esp_err_t light_controller_init(void)
{
    ledc_timer_config_t timer = {
        .speed_mode = k_speed_mode,
        .duty_resolution = LEDC_TIMER_13_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = CONFIG_LIGHT_PWM_FREQUENCY,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    ledc_channel_config_t channel = {
        .gpio_num = CONFIG_LIGHT_PWM_GPIO,
        .speed_mode = k_speed_mode,
        .channel = k_channel,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0,
    };
    esp_err_t err = ledc_timer_config(&timer);
    if (err != ESP_OK) {
        return err;
    }
    err = ledc_channel_config(&channel);
    if (err != ESP_OK) {
        return err;
    }
    const esp_timer_create_args_t timer_args = {
        .callback = delayed_off_callback,
        .name = "light_off",
    };
    return esp_timer_create(&timer_args, &s_off_timer);
}

void light_controller_turn_on(void)
{
    s_state.is_on = true;
    apply_output();
}

void light_controller_turn_off(void)
{
    s_state.is_on = false;
    apply_output();
}

void light_controller_adjust_brightness(int8_t delta_percent)
{
    int next = (int)s_state.brightness_percent + delta_percent;
    if (next < 1) {
        next = 1;
    } else if (next > 100) {
        next = 100;
    }
    s_state.brightness_percent = (uint8_t)next;
    s_state.is_on = true;
    apply_output();
}

void light_controller_schedule_off(uint32_t delay_hours)
{
    if (delay_hours == 0 || s_off_timer == NULL) {
        return;
    }
    esp_timer_stop(s_off_timer);
    uint64_t delay_us = (uint64_t)delay_hours * 60ULL * 60ULL * 1000000ULL;
    ESP_ERROR_CHECK(esp_timer_start_once(s_off_timer, delay_us));
    ESP_LOGI(TAG, "scheduled switch-off after %lu hour(s)", (unsigned long)delay_hours);
}

light_state_t light_controller_get_state(void)
{
    return s_state;
}
