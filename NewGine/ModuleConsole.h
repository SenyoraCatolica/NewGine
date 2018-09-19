#pragma once
#include "Module.h"
#include <list>

struct ConsoleLog {
	const char* data = NULL;
	bool error = false;
};

class ModuleConsole : public Module
{
	ModuleConsole(Application* app, bool start_enabled = true);
	~ModuleConsole();

	void AddLog(const char* log, bool error = false);
	void ClearLog();

public:
	char InputBuff[256];
	std::list<ConsoleLog*> logs;

};