#pragma once

#include <list>
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "ModuleGOManager.h"
#include "ModuleFBXLoader.h"

using namespace std; 

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	//ModuleAudio* audio;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleEditor* editor;
	ModuleGOManager* go_manager;
	ModuleFBXLoader* fbx_loader;

private:

	Timer	ms_timer;
	float	dt;
	list<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	void RequestBrowser(const char* link);

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
	
};