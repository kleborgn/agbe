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
    {0x03, {in_type::IN_INC, addr_mode::AM_R, reg_type::RT_BC}},
    {0x04, {in_type::IN_INC, addr_mode::AM_R, reg_type::RT_B}},
	{0x05, {in_type::IN_DEC, addr_mode::AM_R, reg_type::RT_B}},
    {0x06, {in_type::IN_LD, addr_mode::AM_R_D8, reg_type::RT_B}},

    {0x08, {in_type::IN_LD, addr_mode::AM_A16_R, reg_type::RT_NONE, reg_type::RT_SP}},
    {0x09, {in_type::IN_ADD, addr_mode::AM_R_R, reg_type::RT_HL, reg_type::RT_BC}},
    {0x0A, {in_type::IN_LD, addr_mode::AM_R_MR, reg_type::RT_A, reg_type::RT_BC}},
    {0x0B, {in_type::IN_DEC, addr_mode::AM_R, reg_type::RT_BC}},
    {0x0C, {in_type::IN_INC, addr_mode::AM_R, reg_type::RT_C}},
    {0x0D, {in_type::IN_DEC, addr_mode::AM_R, reg_type::RT_C}},
	{0x0E, {in_type::IN_LD, addr_mode::AM_R_D8, reg_type::RT_C}},

    //0x1X
    {0x11, {in_type::IN_LD, addr_mode::AM_R_D16, reg_type::RT_DE}},
    {0x12, {in_type::IN_LD, addr_mode::AM_MR_R, reg_type::RT_DE, reg_type::RT_A}},
    {0x13, {in_type::IN_INC, addr_mode::AM_R, reg_type::RT_DE}},
    {0x14, {in_type::IN_INC, addr_mode::AM_R, reg_type::RT_D}},
    {0x15, {in_type::IN_DEC, addr_mode::AM_R, reg_type::RT_D}},
    {0x16, {in_type::IN_LD, addr_mode::AM_R_D8, reg_type::RT_D}},

    {0x18, {in_type::IN_JR, addr_mode::AM_D8}},
    {0x19, {in_type::IN_ADD, addr_mode::AM_R_R, reg_type::RT_HL, reg_type::RT_DE}},
    {0x1A, {in_type::IN_LD, addr_mode::AM_R_MR, reg_type::RT_A, reg_type::RT_DE}},
    {0x1B, {in_type::IN_DEC, addr_mode::AM_R, reg_type::RT_DE}},
    {0x1C, {in_type::IN_INC, addr_mode::AM_R, reg_type::RT_E}},
    {0x1D, {in_type::IN_DEC, addr_mode::AM_R, reg_type::RT_E}},
    {0x1E, {in_type::IN_LD, addr_mode::AM_R_D8, reg_type::RT_E}},

    //0x2X
    {0x20, {in_type::IN_JR, addr_mode::AM_D8, reg_type::RT_NONE, reg_type::RT_NONE, cond_type::CT_NZ}},
    {0x21, {in_type::IN_LD, addr_mode::AM_R_D16, reg_type::RT_HL}},
    {0x22, {in_type::IN_LD, addr_mode::AM_HLI_R, reg_type::RT_HL, reg_type::RT_A}},
    {0x23, {in_type::IN_INC, addr_mode::AM_R, reg_type::RT_HL}},
	{0x24, {in_type::IN_INC, addr_mode::AM_R, reg_type::RT_H}},
    {0x25, {in_type::IN_DEC, addr_mode::AM_R, reg_type::RT_H}},
    {0x26, {in_type::IN_LD, addr_mode::AM_R_D8, reg_type::RT_H}},

    {0x28, {in_type::IN_JR, addr_mode::AM_D8, reg_type::RT_NONE, reg_type::RT_NONE, cond_type::CT_Z}},
    {0x29, {in_type::IN_ADD, addr_mode::AM_R_R, reg_type::RT_HL, reg_type::RT_HL}},
    {0x2A, {in_type::IN_LD, addr_mode::AM_R_HLI, reg_type::RT_A, reg_type::RT_HL}},
    {0x2B, {in_type::IN_DEC, addr_mode::AM_R, reg_type::RT_HL}},
    {0x2C, {in_type::IN_INC, addr_mode::AM_R, reg_type::RT_L}},
    {0x2D, {in_type::IN_DEC, addr_mode::AM_R, reg_type::RT_L}},
    {0x2E, {in_type::IN_LD, addr_mode::AM_R_D8, reg_type::RT_L}},

    //0x3X
    {0x30, {in_type::IN_JR, addr_mode::AM_D8, reg_type::RT_NONE, reg_type::RT_NONE, cond_type::CT_NC}},
    {0x31, {in_type::IN_LD, addr_mode::AM_R_D16, reg_type::RT_SP}},
    {0x32, {in_type::IN_LD, addr_mode::AM_HLD_R, reg_type::RT_HL, reg_type::RT_A}},
    {0x33, {in_type::IN_INC, addr_mode::AM_R, reg_type::RT_SP}},
    {0x34, {in_type::IN_INC, addr_mode::AM_MR, reg_type::RT_HL}},
    {0x35, {in_type::IN_DEC, addr_mode::AM_R, reg_type::RT_HL}},
    {0x36, {in_type::IN_LD, addr_mode::AM_MR_D8, reg_type::RT_HL}},

    {0x38, {in_type::IN_JR, addr_mode::AM_D8, reg_type::RT_NONE, reg_type::RT_NONE, cond_type::CT_C}},
    {0x39, {in_type::IN_ADD, addr_mode::AM_R_R, reg_type::RT_HL, reg_type::RT_SP}},
    {0x3A, {in_type::IN_LD, addr_mode::AM_R_HLD, reg_type::RT_A, reg_type::RT_HL}},
    {0x3B, {in_type::IN_DEC, addr_mode::AM_R, reg_type::RT_SP}},
    {0x3C, {in_type::IN_INC, addr_mode::AM_R, reg_type::RT_A}},
    {0x3D, {in_type::IN_DEC, addr_mode::AM_R, reg_type::RT_A}},
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
    {0x70, {in_type::IN_LD, addr_mode::AM_MR_R, reg_type::RT_HL, reg_type::RT_B}},
    {0x71, {in_type::IN_LD, addr_mode::AM_MR_R, reg_type::RT_HL, reg_type::RT_C}},
    {0x72, {in_type::IN_LD, addr_mode::AM_MR_R, reg_type::RT_HL, reg_type::RT_D}},
    {0x73, {in_type::IN_LD, addr_mode::AM_MR_R, reg_type::RT_HL, reg_type::RT_E}},
    {0x74, {in_type::IN_LD, addr_mode::AM_MR_R, reg_type::RT_HL, reg_type::RT_H}},
    {0x75, {in_type::IN_LD, addr_mode::AM_MR_R, reg_type::RT_HL, reg_type::RT_L}},
    {0x76, {in_type::IN_HALT}},
    {0x77, {in_type::IN_LD, addr_mode::AM_MR_R, reg_type::RT_HL, reg_type::RT_A}},
    {0x78, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_B}},
    {0x79, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_C}},
    {0x7A, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_D}},
    {0x7B, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_E}},
    {0x7C, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_H}},
    {0x7D, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_L}},
    {0x7E, {in_type::IN_LD, addr_mode::AM_R_MR, reg_type::RT_A, reg_type::RT_HL}},
    {0x7F, {in_type::IN_LD, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_A}},

	//0x8X
    {0x80, {in_type::IN_ADD, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_B}},
    {0x81, {in_type::IN_ADD, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_C}},
    {0x82, {in_type::IN_ADD, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_D}},
    {0x83, {in_type::IN_ADD, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_E}},
    {0x84, {in_type::IN_ADD, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_H}},
    {0x85, {in_type::IN_ADD, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_L}},
    {0x86, {in_type::IN_ADD, addr_mode::AM_R_MR, reg_type::RT_A, reg_type::RT_HL}},
    {0x87, {in_type::IN_ADD, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_A}},
    {0x88, {in_type::IN_ADC, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_B}},
    {0x89, {in_type::IN_ADC, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_C}},
    {0x8A, {in_type::IN_ADC, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_D}},
    {0x8B, {in_type::IN_ADC, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_E}},
    {0x8C, {in_type::IN_ADC, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_H}},
    {0x8D, {in_type::IN_ADC, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_L}},
    {0x8E, {in_type::IN_ADC, addr_mode::AM_R_MR, reg_type::RT_A, reg_type::RT_HL}},
    {0x8F, {in_type::IN_ADC, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_A}},

    //0x9X
    {0x90, {in_type::IN_SUB, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_B}},
    {0x91, {in_type::IN_SUB, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_C}},
    {0x92, {in_type::IN_SUB, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_D}},
    {0x93, {in_type::IN_SUB, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_E}},
    {0x94, {in_type::IN_SUB, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_H}},
    {0x95, {in_type::IN_SUB, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_L}},
    {0x96, {in_type::IN_SUB, addr_mode::AM_R_MR, reg_type::RT_A, reg_type::RT_HL}},
    {0x97, {in_type::IN_SUB, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_A}},
    {0x98, {in_type::IN_SBC, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_B}},
    {0x99, {in_type::IN_SBC, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_C}},
    {0x9A, {in_type::IN_SBC, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_D}},
    {0x9B, {in_type::IN_SBC, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_E}},
    {0x9C, {in_type::IN_SBC, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_H}},
    {0x9D, {in_type::IN_SBC, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_L}},
    {0x9E, {in_type::IN_SBC, addr_mode::AM_R_MR, reg_type::RT_A, reg_type::RT_HL}},
    {0x9F, {in_type::IN_SBC, addr_mode::AM_R_R, reg_type::RT_A, reg_type::RT_A}},

	//0xAX
	{0xAF, {in_type::IN_XOR, addr_mode::AM_R, reg_type::RT_A}},

    //0xCX
    {0xC0, {in_type::IN_RET, addr_mode::AM_IMP, reg_type::RT_NONE, reg_type::RT_NONE, cond_type::CT_NZ}},
    {0xC1, {in_type::IN_POP, addr_mode::AM_IMP, reg_type::RT_BC}},
    {0xC2, {in_type::IN_JP, addr_mode::AM_D16, reg_type::RT_NONE, reg_type::RT_NONE, cond_type::CT_NZ}},
	{0xC3, {in_type::IN_JP, addr_mode::AM_D16}},
    {0xC4, {in_type::IN_CALL, addr_mode::AM_D16, reg_type::RT_NONE, reg_type::RT_NONE, cond_type::CT_NZ}},
    {0xC5, {in_type::IN_PUSH, addr_mode::AM_IMP, reg_type::RT_BC}},
	{0xC6, {in_type::IN_ADD, addr_mode::AM_R_A8, reg_type::RT_A}},
    {0xC7, {in_type::IN_RST, addr_mode::AM_IMP, reg_type::RT_NONE, reg_type::RT_NONE, cond_type::CT_NONE, 0x00}},
    {0xC8, {in_type::IN_RET, addr_mode::AM_IMP, reg_type::RT_NONE, reg_type::RT_NONE, cond_type::CT_Z}},
    {0xC9, {in_type::IN_RET}},
    {0xCA, {in_type::IN_JP, addr_mode::AM_D16, reg_type::RT_NONE, reg_type::RT_NONE, cond_type::CT_Z} },

    {0xCC, {in_type::IN_CALL, addr_mode::AM_D16, reg_type::RT_NONE, reg_type::RT_NONE, cond_type::CT_Z}},
	{0xCD, {in_type::IN_CALL, addr_mode::AM_D16}},
	{0xCE, {in_type::IN_ADC, addr_mode::AM_R_D8, reg_type::RT_A}},
    {0xCF, {in_type::IN_RST, addr_mode::AM_IMP, reg_type::RT_NONE, reg_type::RT_NONE, cond_type::CT_NONE, 0x08}},

    //0xDX
    {0xD0, {in_type::IN_RET, addr_mode::AM_IMP, reg_type::RT_NONE, reg_type::RT_NONE, cond_type::CT_NC}},
    {0xD1, {in_type::IN_POP, addr_mode::AM_IMP, reg_type::RT_DE}},
    {0xD2, {in_type::IN_JP, addr_mode::AM_D16, reg_type::RT_NONE, reg_type::RT_NONE, cond_type::CT_NC}},

    {0xD4, {in_type::IN_CALL, addr_mode::AM_D16, reg_type::RT_NONE, reg_type::RT_NONE, cond_type::CT_NC}},
    {0xD5, {in_type::IN_PUSH, addr_mode::AM_IMP, reg_type::RT_DE}},

    {0xD7, {in_type::IN_RST, addr_mode::AM_IMP, reg_type::RT_NONE, reg_type::RT_NONE, cond_type::CT_NONE, 0x10}},
    {0xD8, {in_type::IN_RET, addr_mode::AM_IMP, reg_type::RT_NONE, reg_type::RT_NONE, cond_type::CT_C}},
    {0xD9, {in_type::IN_RETI}},
    {0xDA, {in_type::IN_JP, addr_mode::AM_D16, reg_type::RT_NONE, reg_type::RT_NONE, cond_type::CT_C}},

	{0xDC, {in_type::IN_CALL, addr_mode::AM_D16, reg_type::RT_NONE, reg_type::RT_NONE, cond_type::CT_C}},

    {0xDF, {in_type::IN_RST, addr_mode::AM_IMP, reg_type::RT_NONE, reg_type::RT_NONE, cond_type::CT_NONE, 0x18}},

	//0xEX
    {0xE0, {in_type::IN_LDH, addr_mode::AM_A8_R, reg_type::RT_NONE, reg_type::RT_A}},
    {0xE1, {in_type::IN_POP, addr_mode::AM_IMP, reg_type::RT_HL}},
    {0xE2, {in_type::IN_LD, addr_mode::AM_MR_R, reg_type::RT_C, reg_type::RT_A}},

    {0xE5, {in_type::IN_PUSH, addr_mode::AM_IMP, reg_type::RT_HL}},

    {0xE7, {in_type::IN_RST, addr_mode::AM_IMP, reg_type::RT_NONE, reg_type::RT_NONE, cond_type::CT_NONE, 0x20}},
	{0xE8, {in_type::IN_ADD, addr_mode::AM_R_D8, reg_type::RT_SP}},
    {0xE9, {in_type::IN_JP, addr_mode::AM_MR, reg_type::RT_HL}},
    {0xEA, {in_type::IN_LD, addr_mode::AM_A16_R, reg_type::RT_NONE, reg_type::RT_A}},

    {0xEF, {in_type::IN_RST, addr_mode::AM_IMP, reg_type::RT_NONE, reg_type::RT_NONE, cond_type::CT_NONE, 0x28}},

    //0xFX
    {0xF0, {in_type::IN_LDH, addr_mode::AM_R_A8, reg_type::RT_A}},
    {0xF1, {in_type::IN_POP, addr_mode::AM_IMP, reg_type::RT_AF}},
    {0xF2, {in_type::IN_LD, addr_mode::AM_R_MR, reg_type::RT_A, reg_type::RT_C}},
	{0xF3, {in_type::IN_DI}},

    {0xF5, {in_type::IN_PUSH, addr_mode::AM_IMP, reg_type::RT_AF}},

    {0xF7, {in_type::IN_RST, addr_mode::AM_IMP, reg_type::RT_NONE, reg_type::RT_NONE, cond_type::CT_NONE, 0x30}},

    {0xFA, {in_type::IN_LD, addr_mode::AM_R_A16, reg_type::RT_A}},

    {0xFF, {in_type::IN_RST, addr_mode::AM_IMP, reg_type::RT_NONE, reg_type::RT_NONE, cond_type::CT_NONE, 0x38}},
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