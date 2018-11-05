#include "Window.h"


Window::Window(Application* app) : App(app) {};

void Window::SetActive(bool value)
{
	active = value;
}

bool Window::GetActive()
{
	return active;
}