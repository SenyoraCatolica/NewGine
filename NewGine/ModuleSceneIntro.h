#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"



class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	

public:
	Plane_P* grid;
	
};
