#pragma once

#include <cstdint>

typedef struct {
	bool bPaused;
	bool bRunning;
	bool bDie;
	uint64_t ticks;
} emu_context;

int emu_run(int argc, char** argv);

emu_context* emu_get_context();

void emu_cycles(int nb_cycles);