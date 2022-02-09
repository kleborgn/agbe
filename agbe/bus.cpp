#include "stdafx.h"
#include "bus.h"

#include <cstdio>

#include "cart.h"
#include "cpu.h"
#include "ram.h"

/*
 * 0000-3FFF	16 KiB ROM bank 00 ; From cartridge, usually a fixed bank
 * 4000-7FFF	16 KiB ROM Bank 01~NN ; From cartridge, switchable bank via mapper (if any)
 * 8000-9FFF	8 KiB Video RAM (VRAM) ; In CGB mode, switchable bank 0/1
 * A000-BFFF	8 KiB External RAM ; From cartridge, switchable bank if any
 * C000-CFFF	4 KiB Work RAM (WRAM)	
 * D000-DFFF	4 KiB Work RAM (WRAM) ; In CGB mode, switchable bank 1~7
 * E000-FDFF	Mirror of C000~DDFF (ECHO RAM) ; Nintendo says use of this area is prohibited.
 * FE00-FE9F	Sprite attribute table (OAM)	
 * FEA0-FEFF	Not Usable ; Nintendo says use of this area is prohibited
 * FF00-FF7F	I/O Registers	
 * FF80-FFFE	High RAM (HRAM)	
 * FFFF-FFFF	Interrupt Enable register (IE)	
 */

uint8_t bus_read(uint16_t address)
{
	if(address < 0x8000) //ROM
	{
		return cart_read(address);
	} else if (address < 0xA000) //char and map data
	{
		printf("[-] Unsupported: bus_read(%04X)\n", address);
		NOT_IMPLEMENTED
	} else if (address < 0xC000) //cartridge RAM
	{
			return cart_read(address);
	} else if (address < 0xE000) //WRAM
	{
		return wram_read(address);
	} else if (address < 0xFE00) //Echo RAM (not usable)
	{
		return 0;
	} else if (address < 0xFEA0) //OAM
	{
		printf("[-] Unsupported: bus_read(%04X)\n", address);
		NOT_IMPLEMENTED
	} else if (address < 0xFF00) //Not usable
	{
		return 0;
	} else if (address < 0xFF80) //IO regs
	{
		printf("[-] Unsupported: bus_read(%04X)\n", address);
		//NOT_IMPLEMENTED
	} else if (address == 0xFFFF) //IE
	{
		return cpu_get_ie_register();
	}

		return hram_read(address);
}

void bus_write(uint16_t address, uint8_t value)
{
	if (address < 0x8000) //ROM
	{
		cart_write(address, value);
	}
	else if (address < 0xA000) //char and map data
	{
		printf("[-] Unsupported: bus_write(%04X)\n", address);
		NOT_IMPLEMENTED
	}
	else if (address < 0xC000) //cartridge RAM
	{
		cart_write(address, value);
	}
	else if (address < 0xE000) //WRAM
	{
		wram_write(address, value);
	}
	else if (address < 0xFE00) //Echo RAM (not usable)
	{
		
	}
	else if (address < 0xFEA0) //OAM
	{
		printf("[-] Unsupported: bus_write(%04X)\n", address);
		NOT_IMPLEMENTED
	}
	else if (address < 0xFF00) //Not usable
	{
		
	}
	else if (address < 0xFF80) //IO regs
	{
		printf("[-] Unsupported: bus_write(%04X)\n", address);
		//NOT_IMPLEMENTED
	}
	else if (address == 0xFFFF) //IE
	{
		cpu_set_ie_register(value);
	}
	else
	{
		hram_write(address, value);
	}
}

uint16_t bus_read16(uint16_t address)
{
	uint16_t lo = bus_read(address);
	uint16_t hi = bus_read(address + 1);

	return lo | (hi << 8);
}

void bus_write16(uint16_t address, uint16_t value)
{
	bus_write(address + 1, (value >> 8) & 0xFF);
	bus_write(address, value & 0xFF);
}



