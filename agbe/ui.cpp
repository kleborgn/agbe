#include "stdafx.h"
#include "ui.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include "emu.h"

SDL_Window* sdl_window;
SDL_Renderer* sdl_renderer;
SDL_Texture* sdl_texture;
SDL_Surface* screen;

void delay(uint32_t ms)
{
	SDL_Delay(ms);
}

void ui_init()
{
	SDL_Init(SDL_INIT_VIDEO);
	std::cout << "[+] SDL init done." << std::endl;

	TTF_Init();
	std::cout << "[+] SDL TTF init done." << std::endl;

	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &sdl_window, &sdl_renderer);
}

void ui_handle_events()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) > 0)
	{
		if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE)
		{
			emu_get_context()->bDie = true;
		}
	}
}
