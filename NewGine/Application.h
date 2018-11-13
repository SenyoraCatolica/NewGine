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
#include "ModuleResourceManager.h"
#include "ModuleImporter.h"
#include "ModuleConsole.h"
#include "ModuleFileSystem.h"

using namespace std;

enum GAME_STATE
{
	PLAY_STATE, STOP_STATE, PAUSED_STATE
};

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
	ModuleResourceManager* resource_manager;
	ModuleImporter* importer;
	ModuleConsole* console;
	ModuleFileSystem* file_system;

private:

	Timer	ms_timer;
	Timer game_timer;
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

	GAME_STATE game_state;

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

	//Game states
	void Play();
	void Stop();
	void Pause();
	GAME_STATE GetGameState();

	double GetDeltaTime();

	bool LoadConfig();
	bool SaveConfig();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
	
};

extern Application* App;
