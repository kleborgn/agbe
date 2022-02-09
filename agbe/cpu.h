#pragma once
#include <cstdint>

#include "instructions.h"

#define BIT(a, n) ((a & (1 << n)) ? 1 : 0)

#define BIT_SET(a, n, on) { if (on) a |= (1 << n); else a &= ~(1 << n);}

#define BETWEEN(a, b, c) ((a >= b) && (a <= c))

#define CPU_FLAG_Z BIT(ctx->regs.f, 7)
#define CPU_FLAG_C BIT(ctx->regs.f, 4)

#define NOT_IMPLEMENTED { fprintf(stderr, "NOT YET IMPLEMENTED\n"); exit(-5); }

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

	bool bInterrupt_master_enabled;
	uint8_t ie_register;
} cpu_context;

void cpu_init();
bool cpu_step();

typedef void (*IN_PROC)(cpu_context*);

IN_PROC inst_get_processor(in_type type);

uint16_t cpu_read_reg(reg_type rt);
void cpu_set_reg(reg_type rt, uint16_t val);

void cpu_set_flags(cpu_context* ctx, char z, char n, char h, char c);

uint8_t cpu_get_ie_register();
void cpu_set_ie_register(uint8_t value);