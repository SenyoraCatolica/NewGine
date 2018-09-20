#pragma once
#include "Module.h"
#include <list>

class ModuleConsole : public Module
{
public:
	ModuleConsole(Application* app, bool start_enabled = true);
	~ModuleConsole();

	void AddMessage(const char* message);
	void ClearLog();

public:
	std::list<const char*> messages;

};