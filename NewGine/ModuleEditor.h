#pragma once
#include "Module.h"
#include "Globals.h"
#include "GameObject.h"

class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Start();

	update_status Update(float dt);


	bool CleanUp();
};
