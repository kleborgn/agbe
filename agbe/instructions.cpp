#include "instructions.h"

#include <iostream>
#include <map>

#include "cpu.h"

std::map<uint32_t, instruction> instructions = {
	{0x00, {IN_NOP, addr_mode::AM_IMP}},
	{0x05, {IN_DEC, addr_mode::AM_R, reg_type::RT_B}},
	{0x0E, {IN_LD, addr_mode::AM_R_D8, reg_type::RT_C}},
	{0xAF, {IN_XOR, addr_mode::AM_R, reg_type::RT_A}},
	{0xC3, {IN_JP, addr_mode::AM_D16}},
	{0xF3, {IN_DI}}
};

instruction* instruction_by_opcode(uint8_t opcode)
{
	try
	{
		return &instructions.at(opcode);
	}
	catch (const std::out_of_range &e)
	{
        fprintf(stderr, "[-] Unknown instruction: %02X\n", opcode);
	}
    return 0;
}

const char* inst_lookup[] = {
    "<NONE>",
    "NOP",
    "LD",
    "INC",
    "DEC",
    "RLCA",
    "ADD",
    "RRCA",
    "STOP",
    "RLA",
    "JR",
    "RRA",
    "DAA",
    "CPL",
    "SCF",
    "CCF",
    "HALT",
    "ADC",
    "SUB",
    "SBC",
    "AND",
    "XOR",
    "OR",
    "CP",
    "POP",
    "JP",
    "PUSH",
    "RET",
    "CB",
    "CALL",
    "RETI",
    "LDH",
    "JPHL",
    "DI",
    "EI",
    "RST",
    "IN_ERR",
    "IN_RLC",
    "IN_RRC",
    "IN_RL",
    "IN_RR",
    "IN_SLA",
    "IN_SRA",
    "IN_SWAP",
    "IN_SRL",
    "IN_BIT",
    "IN_RES",
    "IN_SET"
};

const char* inst_name(in_type t) {
    return inst_lookup[t];
}