#include "stdafx.h"
#include <iostream>
#include <map>

#include "bus.h"
#include "cpu.h"
#include "emu.h"

static void proc_none(cpu_context *ctx)
{
	std::cout << "[-] Invalid instruction." << std::endl;
	exit(-7);
}

static void proc_nop(cpu_context* ctx)
{
	//NOP
}

static void proc_di(cpu_context* ctx)
{
	ctx->bInterrupt_master_enabled = false;
}

static void proc_ld(cpu_context* ctx)
{
	if (ctx->bDest)
	{
		if (ctx->cur_inst->reg_2 >= reg_type::RT_AF) // if 16 bits register
		{
			emu_cycles(1);
			bus_write16(ctx->mem_dest, ctx->fetch_data);
		} else
		{
			bus_write(ctx->mem_dest, static_cast<uint8_t>(ctx->fetch_data));
		}
		return;
	}

	if (ctx->cur_inst->mode == addr_mode::AM_HL_SPR)
	{
		uint8_t hflag = (cpu_read_reg(ctx->cur_inst->reg_2) & 0xF) + (ctx->fetch_data & 0xF) >= 0x10;
		uint8_t cflag = (cpu_read_reg(ctx->cur_inst->reg_2) & 0xFF) + (ctx->fetch_data & 0xFF) >= 0x100;

		cpu_set_flags(ctx, 0, 0, hflag, cflag);
		cpu_set_reg(ctx->cur_inst->reg_1, cpu_read_reg(ctx->cur_inst->reg_2) + static_cast<char>(ctx->fetch_data));

		return;
	}
	cpu_set_reg(ctx->cur_inst->reg_1, ctx->fetch_data);
}

static void proc_ldh(cpu_context *ctx)
{
	if (ctx->cur_inst->reg_1 == reg_type::RT_A)
	{
		cpu_set_reg(ctx->cur_inst->reg_1, bus_read(0xFF00 | ctx->fetch_data));
	} else
	{
		bus_write(0xFF00 | ctx->fetch_data, ctx->regs.a);
	}

	emu_cycles(1);
}

void cpu_set_flags(cpu_context *ctx, char z, char n, char h, char c)
{
	if (z != -1)
	{
		BIT_SET(ctx->regs.f, 7, z);
	}

	if (n != -1)
	{
		BIT_SET(ctx->regs.f, 6, n);
	}

	if (h != -1)
	{
		BIT_SET(ctx->regs.f, 5, h);
	}

	if (c != -1)
	{
		BIT_SET(ctx->regs.f, 4, c);
	}
}

static void proc_xor(cpu_context *ctx)
{
	ctx->regs.a ^= ctx->fetch_data & 0xFF;
	cpu_set_flags(ctx, ctx->regs.a, 0, 0, 0);
}

static bool check_cond(cpu_context* ctx)
{
	bool z = CPU_FLAG_Z;
	bool c = CPU_FLAG_C;

	switch (ctx->cur_inst->cond)
	{
		case cond_type::CT_NONE: return true;
		case cond_type::CT_C: return c;
		case cond_type::CT_NC: return !c;
		case cond_type::CT_Z: return z;
		case cond_type::CT_NZ: return !z;
	}
	return false;
}

static void proc_jp(cpu_context* ctx)
{
	if (check_cond(ctx))
	{
		ctx->regs.pc = ctx->fetch_data;
		emu_cycles(1);
	}
}

static std::map<in_type, IN_PROC> processors = {
	{in_type::IN_NONE, proc_none},
	{in_type::IN_NOP, proc_nop},
	{in_type::IN_LD, proc_ld},
	{in_type::IN_LDH, proc_ldh},
	{in_type::IN_JP, proc_jp},
	{in_type::IN_DI, proc_di},
	{in_type::IN_XOR, proc_xor}
};

IN_PROC inst_get_processor(in_type type)
{
	try
	{
		return processors.at(type);
	}
	catch (std::out_of_range)
	{
		std::cerr << "[-] Unknown IN_TYPE: " << type << std::endl;
	}
	return nullptr;
}
