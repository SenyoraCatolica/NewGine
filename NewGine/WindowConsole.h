#pragma once

#include "Window.h"
#include <list>

class WindowConsole : public Window
{
public:

	struct Log {
		const char* data = NULL;
		bool error = false;
	};

	WindowConsole(Application* app);
	~WindowConsole();

	void AddLog(const char* log, bool error = false);
	void ClearLog();

	void DrawOnEditor();


private:
	char InputBuff[256];
	bool ScrollToBottom;
	std::list<Log*> Logs;
};

extern WindowConsole* console;
