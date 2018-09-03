#pragma once
#include "Window.h"
#include "Globals.h"
#include "SDL\include\SDL.h"


class WindowHardware : public Window
{
public:
	WindowHardware(Application* app);
	~WindowHardware();

	bool Start();
	bool CleanUp();
	void DrawOnEditor();

private:
	SDL_version sdl_vers;
	int cpu_numbers = 0;
	int cpu_cache = 0;
	int system_ram = 0;
};