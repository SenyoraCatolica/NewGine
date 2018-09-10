#pragma once

#include "Module.h"
#include "Globals.h"
#include <map>

class ModuleResourceManager : public Module
{
	ModuleResourceManager(Application* app, bool start_enabled = true);
	~ModuleResourceManager();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
};
