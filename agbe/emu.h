#pragma once

#include <cstdint>

typedef struct {
	bool bPaused;
	bool bRunning;
	uint64_t ticks;
} emu_context;

int emu_run(int argc, char** argv);

emu_context* emu_get_context();
