#pragma once
#include <cstdint>

static const int SCREEN_WIDTH = 1024;
static const int SCREEN_HEIGHT = 768;

void ui_init();
void ui_handle_events();
void delay(uint32_t ms);