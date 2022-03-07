#include "stdafx.h"
#include <iostream>
#include <map>

#include "bus.h"
#include "cpu.h"
#include "emu.h"
#include "stack.h"

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

static void proc_ei(cpu_context *ctx)
{
	ctx->bIME = true;
}

static bool is_16_bits(reg_type rt)
{
	return rt >= reg_type::RT_AF;
}

static void proc_ld(cpu_context* ctx)
{
	if (ctx->bDest)
	{
		if (is_16_bits(ctx->cur_inst->reg_2)) // if 16 bits register
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
		const uint8_t hflag = (cpu_read_reg(ctx->cur_inst->reg_2) & 0xF) + (ctx->fetch_data & 0xF) >= 0x10;
		const uint8_t cflag = (cpu_read_reg(ctx->cur_inst->reg_2) & 0xFF) + (ctx->fetch_data & 0xFF) >= 0x100;

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

static bool check_cond(cpu_context* ctx)
{
	const bool z = CPU_FLAG_Z;
	const bool c = CPU_FLAG_C;

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

static void goto_addr(cpu_context *ctx, uint16_t address, bool bPushPc)
{
	if (check_cond(ctx))
	{
		if (bPushPc)
		{
			emu_cycles(2);
			stack_push16(ctx->regs.pc);
		}
		ctx->regs.pc = address;
		emu_cycles(1);
	}
}

static void proc_jp(cpu_context* ctx)
{
	goto_addr(ctx, ctx->fetch_data, false);
}

static void proc_jr(cpu_context* ctx)
{
	const char rel = static_cast<char>(ctx->fetch_data & 0xFF); // casting cause dest can be negative (to go backward)
	const uint16_t addr = ctx->regs.pc + rel;
	goto_addr(ctx, addr, false);
}

static void proc_call(cpu_context* ctx)
{
	goto_addr(ctx, ctx->fetch_data, true);
}

static void proc_rst(cpu_context* ctx)
{
	goto_addr(ctx, ctx->cur_inst->param, true);
}

static void proc_ret(cpu_context* ctx)
{
	if (ctx->cur_inst->cond != cond_type::CT_NONE)
	{
		emu_cycles(1);
	}

	if (check_cond(ctx))
	{
		const uint16_t lo = stack_pop();
		emu_cycles(1);
		const uint16_t hi = stack_pop();
		emu_cycles(1);

		const uint16_t n = (hi << 8) | lo;
		ctx->regs.pc = n;

		emu_cycles(1);
	}
}

static void proc_reti(cpu_context *ctx)
{
	ctx->bInterrupt_master_enabled = true;
	proc_ret(ctx);
}

static void proc_pop(cpu_context *ctx)
{
	const uint16_t lo = stack_pop();
	emu_cycles(1);
	const uint16_t hi = stack_pop();
	emu_cycles(1);

	uint16_t n = (hi << 8) | lo;

	cpu_set_reg(ctx->cur_inst->reg_1, n);

	if (ctx->cur_inst->reg_1 == reg_type::RT_AF)
	{
		cpu_set_reg(ctx->cur_inst->reg_1, n & 0xFFF0);
	}
}

static void proc_push(cpu_context* ctx)
{
	const uint8_t hi = (cpu_read_reg(ctx->cur_inst->reg_1) >> 8) & 0xFF;
	emu_cycles(1);
	stack_push(hi);

	const uint8_t lo = cpu_read_reg(ctx->cur_inst->reg_2) & 0xFF;
	emu_cycles(1);
	stack_push(lo);

	emu_cycles(1);
}

static void proc_inc(cpu_context *ctx)
{
	uint16_t val = cpu_read_reg(ctx->cur_inst->reg_1) + 1;

	if (is_16_bits(ctx->cur_inst->reg_1))
	{
		emu_cycles(1);
	}

	if (ctx->cur_inst->reg_1 == reg_type::RT_HL && ctx->cur_inst->mode == addr_mode::AM_MR)
	{
		val = bus_read(cpu_read_reg(reg_type::RT_HL)) + 1;
		val &= 0xFF;
		bus_write(cpu_read_reg(reg_type::RT_HL), val);
	} else
	{
		cpu_set_reg(ctx->cur_inst->reg_1, val);
		val = cpu_read_reg(ctx->cur_inst->reg_1);
	}

	if ((ctx->cur_opcode & 0x03) == 0x03)
	{
		return;
	}

	cpu_set_flags(ctx, val == 0, 0, (val & 0x0F) == 0, -1);
}

static void proc_dec(cpu_context* ctx)
{
	uint16_t val = cpu_read_reg(ctx->cur_inst->reg_1) - 1;

	if (is_16_bits(ctx->cur_inst->reg_1))
	{
		emu_cycles(1);
	}

	if (ctx->cur_inst->reg_1 == reg_type::RT_HL && ctx->cur_inst->mode == addr_mode::AM_MR)
	{
		val = bus_read(cpu_read_reg(reg_type::RT_HL)) - 1;
		bus_write(cpu_read_reg(reg_type::RT_HL), val);
	}
	else
	{
		cpu_set_reg(ctx->cur_inst->reg_1, val);
		val = cpu_read_reg(ctx->cur_inst->reg_1);
	}

	if ((ctx->cur_opcode & 0x0B) == 0x0B)
	{
		return;
	}

	cpu_set_flags(ctx, val == 0, 1, (val & 0x0F) == 0x0F, -1);
}

static void proc_adc(cpu_context *ctx)
{
	uint16_t u = ctx->fetch_data;
	uint16_t a = ctx->regs.a;
	uint16_t c = CPU_FLAG_C;

	ctx->regs.a = (a + u + c) & 0xFF;

	cpu_set_flags(ctx, ctx->regs.a == 0, 0, (a & 0xF) + (u & 0xF) + c > 0xF, a + u + c > 0xFF);
}

static void proc_sub(cpu_context *ctx)
{
	uint16_t val = cpu_read_reg(ctx->cur_inst->reg_1) - ctx->fetch_data;

	int z = val == 0;
	int h = (static_cast<int>(cpu_read_reg(ctx->cur_inst->reg_1)) & 0xF) - (static_cast<int>(ctx->fetch_data) & 0xF) < 0;
	int c = static_cast<int>(cpu_read_reg(ctx->cur_inst->reg_1)) - static_cast<int>(ctx->fetch_data) < 0;

	cpu_set_reg(ctx->cur_inst->reg_1, val);
	cpu_set_flags(ctx, z, 1, h, c);
}

static void proc_sbc(cpu_context* ctx)
{
	uint8_t val = ctx->fetch_data + CPU_FLAG_C;

	int z = cpu_read_reg(ctx->cur_inst->reg_1) - val == 0;
	int h = (static_cast<int>(cpu_read_reg(ctx->cur_inst->reg_1)) & 0xF) - (static_cast<int>(ctx->fetch_data) & 0xF) - ((int)CPU_FLAG_C) < 0;
	int c = static_cast<int>(cpu_read_reg(ctx->cur_inst->reg_1)) - static_cast<int>(ctx->fetch_data) - ((int)CPU_FLAG_C) < 0;

	cpu_set_reg(ctx->cur_inst->reg_1, cpu_read_reg(ctx->cur_inst->reg_1) - val);
	cpu_set_flags(ctx, z, 1, h, c);
}

static void proc_add(cpu_context *ctx)
{
	uint32_t val = cpu_read_reg(ctx->cur_inst->reg_1) + ctx->fetch_data;

	bool is_16bits = is_16_bits(ctx->cur_inst->reg_1);

	if (is_16bits)
	{
		emu_cycles(1);
	}

	if (ctx->cur_inst->reg_1 == reg_type::RT_SP)
	{
		val = cpu_read_reg(ctx->cur_inst->reg_1) + static_cast<char>(ctx->fetch_data);
	}

	int z = (val & 0xFF) == 0;
	int h = (cpu_read_reg(ctx->cur_inst->reg_1) & 0xF) + (ctx->fetch_data & 0xF) >= 0x10;
	int c = (int)(cpu_read_reg(ctx->cur_inst->reg_1) & 0xFF) + (int)(ctx->fetch_data & 0xFF) >= 0x100;

	if (is_16bits)
	{
		z = -1;
		h = (cpu_read_reg(ctx->cur_inst->reg_1) & 0xFFF) + (ctx->fetch_data & 0xFFF) >= 0x1000;
		uint32_t n = static_cast<uint32_t>(cpu_read_reg(ctx->cur_inst->reg_1)) + static_cast<uint32_t>(ctx->fetch_data);
		c = n >= 0x10000;
	}

	if (ctx->cur_inst->reg_1 == reg_type::RT_SP)
	{
		z = 0;
		h = (cpu_read_reg(ctx->cur_inst->reg_1) & 0xF) + (ctx->fetch_data & 0xF) >= 0x10;
		c = (int)(cpu_read_reg(ctx->cur_inst->reg_1) & 0xFF) + (int)(ctx->fetch_data & 0xFF) > 0x100;
	}

	cpu_set_reg(ctx->cur_inst->reg_1, val & 0xFFFF);
	cpu_set_flags(ctx, z, 0, h, c);
}

reg_type rt_lookup[] = {
	reg_type::RT_B,
	reg_type::RT_C,
	reg_type::RT_D,
	reg_type::RT_E,
	reg_type::RT_H,
	reg_type::RT_L,
	reg_type::RT_HL,
	reg_type::RT_A
};

reg_type decode_reg(uint8_t reg)
{
	if (reg > 0b111)
	{
		return reg_type::RT_NONE;
	}

	return rt_lookup[reg];
}

static void proc_cb(cpu_context *ctx)
{
	uint8_t op = ctx->fetch_data;
	reg_type reg = decode_reg(op & 0b111);
	uint8_t bit = (op >> 3) & 0b111;
	uint8_t bit_op = (op >> 6) & 0b11;
	uint8_t reg_val = cpu_read_reg8(reg);

	emu_cycles(1);

	if (reg == reg_type::RT_HL)
	{
		emu_cycles(2);
	}

	switch (bit_op)
	{
	case 1:
		//BIT
		cpu_set_flags(ctx, !(reg_val & (1 << bit)), 0, 1, -1);
		return;
	case 2:
		//RST
		reg_val &= ~(1 << bit);
		cpu_set_reg8(reg, reg_val);
		return;
	case 3:
		//SET
		reg_val |= (1 << bit);
		cpu_set_reg8(reg, reg_val);
		return;
	}

	bool flagc = CPU_FLAG_C;

	switch (bit)
	{
	case 0:
	{
		//RLC, rotate left
		bool setc = false;
		uint8_t result = (reg_val << 1) & 0xFF;

			if ((reg_val & (1 << 7)) != 0)
			{
				result |= 1;
				setc = true;
			}

			cpu_set_reg8(reg, result);
			cpu_set_flags(ctx, result == 0, false, false, setc);
	} return;
	case 1:
	{
		//RRC, rotate right
		uint8_t old = reg_val;
		reg_val >>= 1;
		reg_val |= (old << 7);

		cpu_set_reg8(reg, reg_val);
		cpu_set_flags(ctx, !reg_val, false, false, old & 1);
	} return;
	case 2:
	{
		//RL, rotate left through carry
		uint8_t old = reg_val;
		reg_val <<= 1;
		reg_val |= flagc;

		cpu_set_reg8(reg, reg_val);
		cpu_set_flags(ctx, !reg_val, false, false, !!(old & 0x80));
	} return;
	case 3:
	{
		//RR, rotate right through carry
		uint8_t old = reg_val;
		reg_val >>= 1;

		reg_val |= (flagc << 7);

		cpu_set_reg8(reg, reg_val);
		cpu_set_flags(ctx, !reg_val, false, false, old & 1);
	} return;
	case 4:
	{
		//SLA, shift left arithmetic (b0=0)
		uint8_t old = reg_val;
		reg_val <<= 1;

		cpu_set_reg8(reg, reg_val);
		cpu_set_flags(ctx, !reg_val, false, false, !!(old & 0x80));
	} return;
	case 5:
	{
		//SRA, shift right arithmetic (b7=b7)
		uint8_t u = static_cast<int8_t>(reg_val) >> 1;
		cpu_set_reg8(reg, u);
		cpu_set_flags(ctx, !u, 0, 0, reg_val & 1);
	} return;
	case 6:
	{
		//SWAP, exchange low/hi-nibble
		reg_val = ((reg_val & 0xF0) >> 4) | ((reg_val & 0xF) << 4);
		cpu_set_reg8(reg, reg_val);
		cpu_set_flags(ctx, reg_val == 0, false, false, false);
	} return;
	case 7:
	{
		//SRL, shift right logical (b7=0)
		uint8_t u = reg_val >> 1;
		cpu_set_reg8(reg, u);
		cpu_set_flags(ctx, !u, 0, 0, reg_val & 1);
	} return;
	}

	printf("[-] Invalid CB: %02X\n", op);
	NOT_IMPLEMENTED
}

static void proc_rlca(cpu_context *ctx)
{
	//rotate A left
	uint8_t u = ctx->regs.a;
	bool c = (u >> 7) & 1;
	u = (u << 1) | c;
	ctx->regs.a = u;

	cpu_set_flags(ctx, 0, 0, 0, c);
}

static void proc_rrca(cpu_context* ctx)
{
	//rotate A right
	uint8_t b = ctx->regs.a & 1;
	ctx->regs.a >>= 1;
	ctx->regs.a |= (b << 7);

	cpu_set_flags(ctx, 0, 0, 0, b);
}

static void proc_rla(cpu_context* ctx)
{
	//rotate A left through carry
	uint8_t u = ctx->regs.a;
	uint8_t cf = CPU_FLAG_C;
	uint8_t c = (u >> 7) & 1;

	ctx->regs.a = (u << 1) | cf;
	cpu_set_flags(ctx, 0, 0, 0, c);
}

static void proc_rra(cpu_context* ctx)
{
	//rotate A right through carry
	uint8_t c = CPU_FLAG_C;
	uint8_t newc = ctx->regs.a & 1;

	ctx->regs.a >>= 1;
	ctx->regs.a |= (c << 7);

	cpu_set_flags(ctx, 0, 0, 0, newc);
}

static void proc_stop(cpu_context *ctx)
{
	printf("[+] STOP\n");
	NOT_IMPLEMENTED
}

static void proc_daa(cpu_context *ctx)
{
	uint8_t u = 0;
	int fc = 0;

	if (CPU_FLAG_H || (!CPU_FLAG_N && (ctx->regs.a & 0xF) > 9))
	{
		u = 6;
	}

	if (CPU_FLAG_C || (!CPU_FLAG_N && ctx->regs.a > 0x99))
	{
		u |= 0x60;
		fc = 1;
	}

	ctx->regs.a += CPU_FLAG_N ? -u : u;

	cpu_set_flags(ctx, ctx->regs.a == 0, -1, 0, fc);
}

static void proc_cpl(cpu_context* ctx)
{
	ctx->regs.a = ~ctx->regs.a;
	cpu_set_flags(ctx, -1, 1, 1, -1);
}

static void proc_scf(cpu_context* ctx)
{
	cpu_set_flags(ctx, -1, 0, 0, 1);
}

static void proc_ccf(cpu_context* ctx)
{
	cpu_set_flags(ctx, -1, 0, 0, CPU_FLAG_C ^ 1);
}

static void proc_halt(cpu_context *ctx)
{
	ctx->bHalted = true;
}
static void proc_and(cpu_context *ctx)
{
	ctx->regs.a &= ctx->fetch_data;
	cpu_set_flags(ctx, ctx->regs.a == 0, 0, 1, 0);
}

static void proc_xor(cpu_context* ctx)
{
	ctx->regs.a ^= ctx->fetch_data & 0xFF;
	cpu_set_flags(ctx, ctx->regs.a == 0, 0, 0, 0);
}

static void proc_or(cpu_context* ctx)
{
	ctx->regs.a |= ctx->fetch_data & 0xFF;
	cpu_set_flags(ctx, ctx->regs.a == 0, 0, 0, 0);
}

static void proc_cp(cpu_context *ctx)
{
	int n = static_cast<int>(ctx->regs.a) - static_cast<int>(ctx->fetch_data);

	cpu_set_flags(ctx, n == 0, 1, (static_cast<int>(ctx->regs.a) & 0x0F) - (static_cast<int>(ctx->fetch_data) & 0x0F) < 0, n < 0);
}

static std::map<in_type, IN_PROC> processors = {
	{in_type::IN_NONE, proc_none},
	{in_type::IN_NOP, proc_nop},
	{in_type::IN_LD, proc_ld},
	{in_type::IN_LDH, proc_ldh},
	{in_type::IN_JP, proc_jp},
	{in_type::IN_DI, proc_di},
	{in_type::IN_EI, proc_ei},
	{in_type::IN_POP, proc_pop},
	{in_type::IN_PUSH, proc_push},
	{in_type::IN_JR, proc_jr},
	{in_type::IN_CALL, proc_call},
	{in_type::IN_RET, proc_ret},
	{in_type::IN_RST, proc_rst},
	{in_type::IN_DEC, proc_dec},
	{in_type::IN_INC, proc_inc},
	{in_type::IN_ADD, proc_add},
	{in_type::IN_ADC, proc_adc},
	{in_type::IN_SUB, proc_sub},
	{in_type::IN_SBC, proc_sbc},
	{in_type::IN_AND, proc_and},
	{in_type::IN_OR, proc_or},
	{in_type::IN_CP, proc_cp},
	{in_type::IN_CB, proc_cb},
	{in_type::IN_RRCA, proc_rrca},
	{in_type::IN_RLCA, proc_rlca},
	{in_type::IN_RRA, proc_rra},
	{in_type::IN_RLA, proc_rla},
	{in_type::IN_STOP, proc_stop},
	{in_type::IN_HALT, proc_halt},
	{in_type::IN_DAA, proc_daa},
	{in_type::IN_CPL, proc_cpl},
	{in_type::IN_SCF, proc_scf},
	{in_type::IN_CCF, proc_ccf},
	{in_type::IN_RETI, proc_reti},
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
