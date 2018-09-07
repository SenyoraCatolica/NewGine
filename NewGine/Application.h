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
	int fps = 60;
	int capped_ms = -1;
	int frame_count = 0;
	int last_sec_frame_count = 0;
	Timer last_sec_frame_time;
	uint32_t last_frame_ms = 0;

	std::string name;
	std::string organization;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	void RequestBrowser(const char* link);

	void SetMaxFPS(int max_fps);
	int GetFPS();
	int GetMS();


	bool LoadConfig();
	bool SaveConfig();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
	
};

extern Application* App;
