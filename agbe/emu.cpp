#include "stdafx.h"
#include "emu.h"

#include <iostream>
#include <thread>

#include "cart.h"
#include "cpu.h"
#include "ui.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

static emu_context ctx;

emu_context* emu_get_context()
{
	return &ctx;
}

int cpu_run()
{
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

	ui_init();

	std::thread t1(cpu_run);

	while(!ctx.bDie)
	{
		Sleep(1000);
		ui_handle_events();
	}

	return 0;
}

void emu_cycles(int nb_cycles)
{
	//TODO
}

