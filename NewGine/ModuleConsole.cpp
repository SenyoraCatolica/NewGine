//#include "ModuleConsole.h"
#include "Application.h"
#include "Globals.h"


ModuleConsole::ModuleConsole(Application* app, bool start_enabled) : Module(app, start_enabled) 
{
	name = "console";
}
ModuleConsole::~ModuleConsole()
{
	//ClearLog();
}

void ModuleConsole::ClearLog()
{
	messages.clear();
}

void ModuleConsole::AddMessage(const char* message)
{
	//2DO
	//messages.push_back(message);
	LOG(message);
}