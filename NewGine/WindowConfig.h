#pragma once
#include "Window.h"
#include "SDL\include\SDL.h"
#include <vector>


class WindowConfig : public Window
{
public:

	WindowConfig(Application* App);
	~WindowConfig();

private:

	void DrawOnEditor();

	void Camera();
	void Input();
	void ApplicationUI();
	void Renderer();
	void WindowUI();
	void Hardware();


private:
	//FPS graphs
	int slider_fps;
	std::vector<float> fps;
	std::vector<float> ms;

	//Hardware specs
	SDL_version sdl_vers;
	int cpu_numbers = 0;
	int cpu_cache = 0;
	int system_ram = 0;

public:
	int max_fps = 60;
};