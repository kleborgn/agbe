#include "instructions.h"

#include <map>

#include "cpu.h"

std::map<uint32_t, instruction> instructions = {
	{0x00, {IN_NOP, addr_mode::AM_IMP}},
	{0x05, {IN_DEC, addr_mode::AM_R, reg_type::RT_B}},
	{0x0E, {IN_LD, addr_mode::AM_R_D8, reg_type::RT_C}},
	{0xAF, {IN_XOR, addr_mode::AM_R, reg_type::RT_A}},
	{0xC3, {IN_JP, addr_mode::AM_D16}}
};

instruction* instruction_by_opcode(uint8_t opcode)
{
	if (instructions.at(opcode).type == IN_NONE)
	{
		return nullptr;
	}
	return &instructions.at(opcode);
}

char* inst_name(in_type t)
{
	return nullptr;
}
