#pragma once

#include "Window.h"
#include <list>

struct ConsoleLog {
	const char* data = NULL;
	bool error = false;
};


class WindowConsole : public Window
{
public:

	WindowConsole(Application* app);
	~WindowConsole();

	void DrawOnEditor();

private:
	char InputBuff[256];
	bool ScrollToBottom;
};

