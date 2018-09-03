#pragma once
#include "Window.h"
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


private:
	//FPS graphs
	int slider_fps;
	std::vector<float> fps;
	std::vector<float> ms;

	//Hardware specs
	std::string caps;
	int total_vram;
	int initial_available_vram;
	int available_vram;
	int usage_vram;

public:
	int max_fps = 60;
};