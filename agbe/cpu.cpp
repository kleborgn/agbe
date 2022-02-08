#include "cpu.h"

#include <iostream>

#include "bus.h"
#include "emu.h"

cpu_context ctx = {0};

//utils

uint16_t reverse(uint16_t n) {
	return ((n & 0xFF00) >> 8) | ((n & 0x00FF) << 8);
}

uint16_t cpu_read_reg(const reg_type rt) {
	switch (rt) {
	case reg_type::RT_A: return ctx.regs.a;
	case reg_type::RT_F: return ctx.regs.f;
	case reg_type::RT_B: return ctx.regs.b;
	case reg_type::RT_C: return ctx.regs.c;
	case reg_type::RT_D: return ctx.regs.d;
	case reg_type::RT_E: return ctx.regs.e;
	case reg_type::RT_H: return ctx.regs.h;
	case reg_type::RT_L: return ctx.regs.l;

	case reg_type::RT_AF: return reverse(*((uint16_t*)&ctx.regs.a));
	case reg_type::RT_BC: return reverse(*((uint16_t*)&ctx.regs.b));
	case reg_type::RT_DE: return reverse(*((uint16_t*)&ctx.regs.d));
	case reg_type::RT_HL: return reverse(*((uint16_t*)&ctx.regs.h));

	case reg_type::RT_PC: return ctx.regs.pc;
	case reg_type::RT_SP: return ctx.regs.sp;
	default: return 0;
	}
}

//

void cpu_init() {
	ctx.regs.pc = 0x100; //EP
}

static void fetch_instruction()
{
	ctx.cur_opcode = bus_read(ctx.regs.pc++);
	ctx.cur_inst = instruction_by_opcode(ctx.cur_opcode);

	if(ctx.cur_inst == nullptr)
	{
		printf("[-] Unknown instruction: %02X\n", ctx.cur_inst->mode);
		exit(-7);
	}
}

static void fetch_data()
{
	ctx.mem_dest = 0;
	ctx.bDest = false;

	switch (ctx.cur_inst->mode)
	{
		case addr_mode::AM_IMP: return;

	case addr_mode::AM_R:
		ctx.fetch_data = cpu_read_reg(ctx.cur_inst->reg_1);
		return;

	case addr_mode::AM_R_D8:
		ctx.fetch_data = bus_read(ctx.regs.pc);
		emu_cycles(1);
		ctx.regs.pc++;
		return;

	case addr_mode::AM_D16:
		{
		uint16_t lo = bus_read(ctx.regs.pc);
		emu_cycles(1);

		uint16_t hi = bus_read(ctx.regs.pc + 1);
		emu_cycles(1);

		ctx.fetch_data = lo | (hi << 8);

		ctx.regs.pc += 2;

		return;
		}

	default:
		std::cout << "[-] Unknown addressing mode: " << ctx.cur_inst->mode << std::endl;
		exit(-7);
		return;
	}
}

static void execute()
{
	std::cout << "[-] Not executing atm." << std::endl;
}

bool cpu_step() {
	if (!ctx.bHalted)
	{
		uint16_t pc = ctx.regs.pc;
		fetch_instruction();
		fetch_data();
		printf("[+] Executing instruction: %02X\tPC: %04X\n", ctx.cur_opcode, pc);
		execute();
	}
    return true;
}