#include "stdafx.h"
#include "stack.h"
#include "cpu.h"
#include "bus.h"

void stack_push(uint8_t data)
{
	cpu_get_regs()->sp--;
	bus_write(cpu_get_regs()->sp, data);
}

void stack_push16(uint16_t data)
{
	stack_push((data >> 8) & 0xFF);
	stack_push(data & 0xFF);
}

uint8_t stack_pop()
{
	return bus_read(cpu_get_regs()->sp++);
}

uint16_t stack_pop16()
{
	const uint16_t lo = stack_pop();
	const uint16_t hi = stack_pop();

	return (hi << 8) | lo;
}
