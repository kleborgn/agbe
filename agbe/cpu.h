#pragma once
#include <cstdint>

#include "instructions.h"

typedef struct
{
	uint8_t a;
	uint8_t f;
	uint8_t b;
	uint8_t c;
	uint8_t d;
	uint8_t e;
	uint8_t h;
	uint8_t l;
	uint16_t pc; //program counter
	uint16_t sp; //stack ptr
} cpu_registers;

typedef struct
{
	cpu_registers regs;
	//current
	uint16_t fetch_data;
	uint16_t mem_dest;
	bool bDest;
	uint8_t cur_opcode;
	instruction *cur_inst;

	bool bHalted;
	bool bStepping;
} cpu_context;

void cpu_init();
bool cpu_step();