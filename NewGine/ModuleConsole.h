#pragma once
#include "Module.h"
#include <list>
#include "WindowConsole.h"

struct ConsoleLog;

class ModuleConsole : public Module
{
public:
	ModuleConsole(Application* app, bool start_enabled = true);
	~ModuleConsole();

	void AddMessage(const char* message);
	void ClearLog();

public:
	std::list<ConsoleLog*> messages;

};