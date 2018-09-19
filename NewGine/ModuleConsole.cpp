#include "ModuleConsole.h"
#include "Application.h"
#include "Globals.h"


ModuleConsole::ModuleConsole(Application* app, bool start_enabled = true) : Module(app, start_enabled) 
{
	name = "console";
}
ModuleConsole::~ModuleConsole()
{
	ClearLog();
}

void ModuleConsole::AddLog(const char* log, bool error = false)
{
	ConsoleLog* tmp = new ConsoleLog();
	tmp->data = log;
	tmp->error = error;
	logs.push_back(tmp);

	//2DO add LOG
}

void ModuleConsole::ClearLog()
{
	logs.clear();
}