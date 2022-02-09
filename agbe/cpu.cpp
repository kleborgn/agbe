#include "stdafx.h"
#include "cpu.h"

#include <iostream>

#include "bus.h"
#include "emu.h"
#include "instructions.h"

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

void cpu_set_reg(reg_type rt, uint16_t val)
{
	switch (rt)
	{
	case reg_type::RT_A: ctx.regs.a = val & 0xFF; break;
	case reg_type::RT_B: ctx.regs.b = val & 0xFF; break;
	case reg_type::RT_C: ctx.regs.c = val & 0xFF; break;
	case reg_type::RT_D: ctx.regs.d = val & 0xFF; break;
	case reg_type::RT_E: ctx.regs.e = val & 0xFF; break;
	case reg_type::RT_H: ctx.regs.h = val & 0xFF; break;
	case reg_type::RT_L: ctx.regs.l = val & 0xFF; break;

	case reg_type::RT_AF: *((uint16_t*)&ctx.regs.a) = reverse(val); break;
	case reg_type::RT_BC: *((uint16_t*)&ctx.regs.b) = reverse(val); break;
	case reg_type::RT_DE: *((uint16_t*)&ctx.regs.d) = reverse(val); break;
	case reg_type::RT_HL: *((uint16_t*)&ctx.regs.h) = reverse(val); break;

	case reg_type::RT_PC: ctx.regs.pc = val; break;
	case reg_type::RT_SP: ctx.regs.sp = val; break;
	case reg_type::RT_NONE: break;
	}
}

//

void cpu_init() {
	ctx.regs.pc = 0x100; //EP
	ctx.regs.a = 0x01;
}

static void fetch_instruction()
{
	ctx.cur_opcode = bus_read(ctx.regs.pc++);
	ctx.cur_inst = instruction_by_opcode(ctx.cur_opcode);
}

static void fetch_data()
{
	ctx.mem_dest = 0;
	ctx.bDest = false;

	if (ctx.cur_inst == nullptr)
	{
		return;
	}

	switch (ctx.cur_inst->mode)
	{
		case addr_mode::AM_IMP: return;

	case addr_mode::AM_R:
		ctx.fetch_data = cpu_read_reg(ctx.cur_inst->reg_1);
		return;

	case addr_mode::AM_R_R:
		ctx.fetch_data = cpu_read_reg(ctx.cur_inst->reg_2);
		return;

	case addr_mode::AM_R_D8:
		ctx.fetch_data = bus_read(ctx.regs.pc);
		emu_cycles(1);
		ctx.regs.pc++;
		return;

	case addr_mode::AM_R_D16:
	case addr_mode::AM_D16:
		{
			const uint16_t lo = bus_read(ctx.regs.pc);
		emu_cycles(1);

			const uint16_t hi = bus_read(ctx.regs.pc + 1);
		emu_cycles(1);

		ctx.fetch_data = lo | (hi << 8);

		ctx.regs.pc += 2;

		return;
		}

	case addr_mode::AM_MR_R:
		ctx.fetch_data = cpu_read_reg(ctx.cur_inst->reg_2);
		ctx.mem_dest = cpu_read_reg(ctx.cur_inst->reg_1);
		ctx.bDest = true;

		if (ctx.cur_inst->reg_1 == reg_type::RT_C)
		{
			ctx.mem_dest |= 0xFF00;
		}

		return;

	case addr_mode::AM_R_MR:
		{
		uint16_t addr = cpu_read_reg(ctx.cur_inst->reg_2);

		if (ctx.cur_inst->reg_1 == reg_type::RT_C)
		{
			addr |= 0xFF00;
		}

		ctx.fetch_data = bus_read(addr);
		emu_cycles(1);
		return;
		}

	case addr_mode::AM_R_HLI:
		ctx.fetch_data = bus_read(cpu_read_reg(ctx.cur_inst->reg_2));
		emu_cycles(1);
		cpu_set_reg(reg_type::RT_HL, cpu_read_reg(reg_type::RT_HL) + 1);
		return;

	case addr_mode::AM_R_HLD:
		ctx.fetch_data = bus_read(cpu_read_reg(ctx.cur_inst->reg_2));
		emu_cycles(1);
		cpu_set_reg(reg_type::RT_HL, cpu_read_reg(reg_type::RT_HL) - 1);
		return;

	case addr_mode::AM_HLI_R:
		ctx.fetch_data = cpu_read_reg(ctx.cur_inst->reg_2);
		ctx.mem_dest = cpu_read_reg(ctx.cur_inst->reg_1);
		ctx.bDest = true;
		cpu_set_reg(reg_type::RT_HL, cpu_read_reg(reg_type::RT_HL) + 1);
		return;

	case addr_mode::AM_HLD_R:
		ctx.fetch_data = cpu_read_reg(ctx.cur_inst->reg_2);
		ctx.mem_dest = cpu_read_reg(ctx.cur_inst->reg_1);
		ctx.bDest = true;
		cpu_set_reg(reg_type::RT_HL, cpu_read_reg(reg_type::RT_HL) - 1);
		return;

	case addr_mode::AM_R_A8:
		ctx.fetch_data = bus_read(ctx.regs.pc);
		emu_cycles(1);
		ctx.regs.pc++;
		return;

	case addr_mode::AM_A8_R:
		ctx.mem_dest = bus_read(ctx.regs.pc) | 0xFF00;
		ctx.bDest = true;
		emu_cycles(1);
		ctx.regs.pc++;
		return;

	case addr_mode::AM_HL_SPR:
		ctx.fetch_data = bus_read(ctx.regs.pc);
		emu_cycles(1);
		ctx.regs.pc++;
		return;

	case addr_mode::AM_D8:
		ctx.fetch_data = bus_read(ctx.regs.pc);
		emu_cycles(1);
		ctx.regs.pc++;
		return;

	case addr_mode::AM_A16_R:
	case addr_mode::AM_D16_R:
	{
		const uint16_t lo = bus_read(ctx.regs.pc);
		emu_cycles(1);

		const uint16_t hi = bus_read(ctx.regs.pc + 1);
		emu_cycles(1);

		ctx.mem_dest = lo | (hi << 8);
		ctx.bDest = true;

		ctx.regs.pc += 2;
		ctx.fetch_data = cpu_read_reg(ctx.cur_inst->reg_2);

		return;
	}

	case addr_mode::AM_MR_D8:
		ctx.fetch_data = bus_read(ctx.regs.pc);
		emu_cycles(1);
		ctx.regs.pc++;
		ctx.mem_dest = cpu_read_reg(ctx.cur_inst->reg_1);
		ctx.bDest = true;
		return;

	case addr_mode::AM_MR:
		ctx.mem_dest = cpu_read_reg(ctx.cur_inst->reg_1);
		ctx.bDest = true;
		ctx.fetch_data = bus_read(cpu_read_reg(ctx.cur_inst->reg_1));
		emu_cycles(1);
		return;

	case addr_mode::AM_R_A16:
	{
		const uint16_t lo = bus_read(ctx.regs.pc);
		emu_cycles(1);

		const uint16_t hi = bus_read(ctx.regs.pc + 1);
		emu_cycles(1);

		const uint16_t addr = lo | (hi << 8);

		ctx.regs.pc += 2;
		ctx.fetch_data = bus_read(addr);
		emu_cycles(1);

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
	const IN_PROC proc = inst_get_processor(ctx.cur_inst->type);

	if (!proc)
	{
		NOT_IMPLEMENTED
	}

	proc(&ctx);
}

bool cpu_step() {
	if (!ctx.bHalted)
	{
		const uint16_t pc = ctx.regs.pc;
		fetch_instruction();
		fetch_data();

		printf("%04X: %-7s (%02X %02X %02X) A: %02X BC: %02X%02X DE:%02X%02X HL:%02X%02X\n",
			pc, inst_name(ctx.cur_inst->type), ctx.cur_opcode, bus_read(pc + 1), bus_read(pc + 2), 
			ctx.regs.a, ctx.regs.b, ctx.regs.c, ctx.regs.d, ctx.regs.e, ctx.regs.h, ctx.regs.l);

		//printf("[+] Executing instruction: %02X\tPC: %04X\n", ctx.cur_opcode, pc);

		if (ctx.cur_inst == nullptr)
		{
			printf("[-] Unknown instruction: %02X\n", ctx.cur_opcode);
			_exit(-7);
		}

		execute();
	}
    return true;
}

uint8_t cpu_get_ie_register()
{
	return ctx.ie_register;
}

void cpu_set_ie_register(uint8_t value)
{
	ctx.ie_register = value;
}

