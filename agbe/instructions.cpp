#include "stdafx.h"
#include "instructions.h"

#include <iostream>
#include <map>

#include "cpu.h"

std::map<uint32_t, instruction> instructions = {
    //0x0X
	{0x00, {in_type::IN_NOP, addr_mode::AM_IMP}},
    {0x01, {in_type::IN_LD, addr_mode::AM_R_D16, reg_type::RT_BC}},
    {0x02, {in_type::IN_LD, addr_mode::AM_MR_R, reg_type::RT_BC, reg_type::RT_A}},

	{0x05, {in_type::IN_DEC, addr_mode::AM_R, reg_type::RT_B}},
    {0x06, {in_type::IN_LD, addr_mode::AM_R_D8, reg_type::RT_B}},

    {0x08, {in_type::IN_LD, addr_mode::AM_A16_R, reg_type::RT_NONE, reg_type::RT_SP}},

    {0x0A, {in_type::IN_LD, addr_mode::AM_R_MR, reg_type::RT_A, reg_type::RT_BC}},

	{0x0E, {in_type::IN_LD, addr_mode::AM_R_D8, reg_type::RT_C}},

    //0x1X
    {0x11, {in_type::IN_LD, addr_mode::AM_R_D16, reg_type::RT_DE}},
    {0x12, {in_type::IN_LD, addr_mode::AM_MR_R, reg_type::RT_DE, reg_type::RT_A}},

    {0x15, {in_type::IN_DEC, addr_mode::AM_R, reg_type::RT_D}},
    {0x16, {in_type::IN_LD, addr_mode::AM_R_D8, reg_type::RT_D}},

    {0x1A, {in_type::IN_LD, addr_mode::AM_R_MR, reg_type::RT_A, reg_type::RT_DE}},

    {0x1E, {in_type::IN_LD, addr_mode::AM_R_D8, reg_type::RT_E}},

    //0x2X
    {0x21, {in_type::IN_LD, addr_mode::AM_R_D16, reg_type::RT_HL}},
    {0x22, {in_type::IN_LD, addr_mode::AM_HLI_R, reg_type::RT_HL, reg_type::RT_A}},

    {0x25, {in_type::IN_DEC, addr_mode::AM_R, reg_type::RT_H}},
    {0x26, {in_type::IN_LD, addr_mode::AM_R_D8, reg_type::RT_H}},

    {0x2A, {in_type::IN_LD, addr_mode::AM_R_HLI, reg_type::RT_A, reg_type::RT_HL}},

    {0x2E, {in_type::IN_LD, addr_mode::AM_R_D8, reg_type::RT_L}},

    //0x3X
    {0x31, {in_type::IN_LD, addr_mode::AM_R_D16, reg_type::RT_SP}},
    {0x32, {in_type::IN_LD, addr_mode::AM_HLD_R, reg_type::RT_HL, reg_type::RT_A}},

    {0x35, {in_type::IN_DEC, addr_mode::AM_R, reg_type::RT_HL}},
    {0x36, {in_type::IN_LD, addr_mode::AM_MR_D8, reg_type::RT_HL}},

    {0x3A, {in_type::IN_LD, addr_mode::AM_R_HLD, reg_type::RT_A, reg_type::RT_HL}},

    {0x3E, {in_type::IN_LD, addr_mode::AM_R_D8, reg_type::RT_A}},

    //0x4X
    {0x40, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_B, reg_type::RT_B}},
    {0x41, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_B, reg_type::RT_C}},
    {0x42, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_B, reg_type::RT_D}},
    {0x43, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_B, reg_type::RT_E}},
    {0x44, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_B, reg_type::RT_H}},
    {0x45, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_B, reg_type::RT_L}},
    {0x46, {in_type::IN_LD, addr_mode::AM_R_MR, reg_type::RT_B, reg_type::RT_HL}},
    {0x47, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_B, reg_type::RT_A}},
    {0x48, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_C, reg_type::RT_B}},
    {0x49, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_C, reg_type::RT_C}},
    {0x4A, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_C, reg_type::RT_D}},
    {0x4B, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_C, reg_type::RT_E}},
    {0x4C, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_C, reg_type::RT_H}},
    {0x4D, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_C, reg_type::RT_L}},
    {0x4E, {in_type::IN_LD, addr_mode::AM_R_MR, reg_type::RT_C, reg_type::RT_HL}},
    {0x4F, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_C, reg_type::RT_A}},

    //0x5X
    {0x50, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_D, reg_type::RT_B}},
    {0x51, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_D, reg_type::RT_C}},
    {0x52, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_D, reg_type::RT_D}},
    {0x53, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_D, reg_type::RT_E}},
    {0x54, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_D, reg_type::RT_H}},
    {0x55, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_D, reg_type::RT_L}},
    {0x56, {in_type::IN_LD, addr_mode::AM_R_MR, reg_type::RT_D, reg_type::RT_HL}},
    {0x57, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_D, reg_type::RT_A}},
    {0x58, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_E, reg_type::RT_B}},
    {0x59, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_E, reg_type::RT_C}},
    {0x5A, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_E, reg_type::RT_D}},
    {0x5B, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_E, reg_type::RT_E}},
    {0x5C, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_E, reg_type::RT_H}},
    {0x5D, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_E, reg_type::RT_L}},
    {0x5E, {in_type::IN_LD, addr_mode::AM_R_MR, reg_type::RT_E, reg_type::RT_HL}},
    {0x5F, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_E, reg_type::RT_A}},

    //0x6X
    {0x60, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_H, reg_type::RT_B}},
    {0x61, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_H, reg_type::RT_C}},
    {0x62, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_H, reg_type::RT_D}},
    {0x63, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_H, reg_type::RT_E}},
    {0x64, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_H, reg_type::RT_H}},
    {0x65, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_H, reg_type::RT_L}},
    {0x66, {in_type::IN_LD, addr_mode::AM_R_MR, reg_type::RT_H, reg_type::RT_HL}},
    {0x67, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_H, reg_type::RT_A}},
    {0x68, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_L, reg_type::RT_B}},
    {0x69, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_L, reg_type::RT_C}},
    {0x6A, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_L, reg_type::RT_D}},
    {0x6B, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_L, reg_type::RT_E}},
    {0x6C, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_L, reg_type::RT_H}},
    {0x6D, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_L, reg_type::RT_L}},
    {0x6E, {in_type::IN_LD, addr_mode::AM_R_MR, reg_type::RT_L, reg_type::RT_HL}},
    {0x6F, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_L, reg_type::RT_A}},

    //0x7X
    { 0x70, {in_type::IN_LD, addr_mode::AM_MR_R, reg_type::RT_HL, reg_type::RT_B}},
    { 0x71, {in_type::IN_LD, addr_mode::AM_MR_R, reg_type::RT_HL, reg_type::RT_C}},
    { 0x72, {in_type::IN_LD, addr_mode::AM_MR_R, reg_type::RT_HL, reg_type::RT_D}},
    { 0x73, {in_type::IN_LD, addr_mode::AM_MR_R, reg_type::RT_HL, reg_type::RT_E}},
    { 0x74, {in_type::IN_LD, addr_mode::AM_MR_R, reg_type::RT_HL, reg_type::RT_H}},
    { 0x75, {in_type::IN_LD, addr_mode::AM_MR_R, reg_type::RT_HL, reg_type::RT_L}},
    { 0x76, {in_type::IN_HALT}},
    { 0x77, {in_type::IN_LD, addr_mode::AM_MR_R, reg_type::RT_HL, reg_type::RT_A}},
    { 0x78, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_B}},
    { 0x79, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_C}},
    { 0x7A, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_D}},
    { 0x7B, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_E}},
    { 0x7C, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_H}},
    { 0x7D, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_L}},
    { 0x7E, {in_type::IN_LD, addr_mode::AM_R_MR, reg_type::RT_A, reg_type::RT_HL}},
    { 0x7F, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_A}},

	//0xAX
	{0xAF, {in_type::IN_XOR, addr_mode::AM_R, reg_type::RT_A}},

    //0xCX
	{0xC3, {in_type::IN_JP, addr_mode::AM_D16}},

	//0xEX
    {0xE0, {in_type::IN_LDH, addr_mode::AM_A8_R, reg_type::RT_NONE, reg_type::RT_A}},
    {0xE2, {in_type::IN_LD, addr_mode::AM_MR_R, reg_type::RT_C, reg_type::RT_A}},
    {0xEA, {in_type::IN_LD, addr_mode::AM_A16_R, reg_type::RT_NONE, reg_type::RT_A}},

    //0xFX
    {0xF0, {in_type::IN_LDH, addr_mode::AM_R_A8, reg_type::RT_A}},
    {0xF2, {in_type::IN_LD, addr_mode::AM_R_MR, reg_type::RT_A, reg_type::RT_C}},
	{0xF3, {in_type::IN_DI}},
    {0xFA, {in_type::IN_LD, addr_mode::AM_R_A16, reg_type::RT_A}},
};

instruction* instruction_by_opcode(uint8_t opcode)
{
	try
	{
		return &instructions.at(opcode);
	}
	catch (std::out_of_range)
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
    return inst_lookup[static_cast<std::underlying_type_t<in_type>>(t)];
}