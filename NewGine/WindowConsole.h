#pragma once

#include "Window.h"
#include "ModuleConsole.h"
#include <list>

class WindowConsole : public Window
{
public:

	WindowConsole(Application* app);
	~WindowConsole();

	void DrawOnEditor();

private:
	char InputBuff[256];
	bool ScrollToBottom;
	std::list<ConsoleLog*> Logs;
};

