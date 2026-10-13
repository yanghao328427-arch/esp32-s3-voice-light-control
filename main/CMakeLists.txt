idf_component_register(
    SRCS
        "app_main.c"
        "light_controller.c"
        "command_router.c"
        "mqtt_bridge.c"
        "voice_commands.c"
    INCLUDE_DIRS "include"
    REQUIRES driver esp_timer mqtt esp_event nvs_flash
)
