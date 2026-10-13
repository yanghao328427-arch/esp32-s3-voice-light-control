#pragma once

#include <stdint.h>

void voice_commands_register(void);
void voice_commands_handle_recognition(uint16_t command_id, const char *recognized_text);
void voice_commands_print_mapping(void);
