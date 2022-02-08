#include "emu.h"

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>

#include "cart.h"
#include "cpu.h"

static emu_context ctx;

emu_context* emu_get_context()
{
	return &ctx;
}

void delay(uint32_t ms)
{
	SDL_Delay(ms);
}

int emu_run(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cout << "Usage: agbe <rom_file>" << std::endl;
		return -1;
	}

	if (!cart_load(argv[1]))
	{
		std::cout << "[-] Failed to load ROM file: " << argv[1] << std::endl;
		return -2;
	}

	std::cout << "[+] ROM file loaded." << std::endl;

	SDL_Init(SDL_INIT_VIDEO);
	std::cout << "[+] SDL init done." << std::endl;

	TTF_Init();
	std::cout << "[+] SDL TTF init done." << std::endl;

	cpu_init();

	ctx.bRunning = true;
	ctx.bPaused = false;
	ctx.ticks = 0;

	while (ctx.bRunning)
	{
		if (ctx.bPaused)
		{
			delay(10);
			continue;
		}

		if (!cpu_step())
		{
			std::cout << "[-] CPU stopped." << std::endl;
			return -3;
		}

		ctx.ticks++;
	}
	return 0;
}

void emu_cycles(int nb_cycles)
{
	//TODO
}

