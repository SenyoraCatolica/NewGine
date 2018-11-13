#include "Application.h"
#include "JSON\parson.h"
#include "Module.h"


Application::Application()
{

	game_timer.Start();

	window = new ModuleWindow(this);
	console = new ModuleConsole(this);
	input = new ModuleInput(this);
	//audio = new ModuleAudio(this, true);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	editor = new ModuleEditor(this);
	go_manager = new ModuleGOManager(this);
	resource_manager = new ModuleResourceManager(this);
	importer = new ModuleImporter(this);
	file_system = new ModuleFileSystem(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(console);
	AddModule(file_system);
	AddModule(camera);
	AddModule(input);
	//AddModule(audio);
	AddModule(go_manager);
	AddModule(resource_manager);
	AddModule(importer);

	
	// Scenes
	AddModule(scene_intro);

	// Renderer last!
	AddModule(renderer3D);
	AddModule(editor);

}

Application::~Application()
{
	list<Module*>::reverse_iterator i = list_modules.rbegin();

	while (i != list_modules.rend())
	{
		delete (*i);
		++i;
	}
}

bool Application::Init()
{
	bool ret = true;

	LoadConfig();


	// Call Init() in all modules
	list<Module*>::iterator i = list_modules.begin();

	while (i != list_modules.end() && ret == true)
	{
		ret = (*i)->Init();
		++i;
	}


	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	i = list_modules.begin();

	while (i != list_modules.end() && ret == true)
	{
		ret = (*i)->Start();
		++i;
	}

	capped_ms = 1000 / fps;

	ms_timer.Start();
	last_sec_frame_time.Start();

	Stop();

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	frame_count++;
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();

	game_timer.UpdateTime();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		last_sec_frame_count = frame_count;
		frame_count = 0;
	}

	last_frame_ms = ms_timer.Read();
	if (capped_ms > 0 && last_frame_ms < capped_ms)
	{
		SDL_Delay(capped_ms - last_frame_ms);
	}
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();

	list<Module*>::iterator i = list_modules.begin();

	while (i != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*i)->PreUpdate();
		++i;
	}

	i = list_modules.begin();

	while (i != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*i)->Update();
		++i;
	}

	i = list_modules.begin();

	while (i != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*i)->PostUpdate();
		i++;
	}


	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	SaveConfig();

	list<Module*>::reverse_iterator i = list_modules.rbegin();

	while (i != list_modules.rend() && ret == true)
	{
		ret = (*i)->CleanUp();
		++i;
	}


	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

void Application::RequestBrowser(const char* link)
{
	ShellExecute(0, 0, link, 0, 0, SW_SHOW);
}

void Application::SetMaxFPS(int max_fps)
{
	fps = max_fps;
	if (fps == 0) fps = -1;
	capped_ms = 1000 / fps;
}

int Application::GetFPS()
{
	return last_sec_frame_count;
}

int Application::GetMS()
{
	return last_frame_ms;
}

//Game states============================================
void Application::Play()
{
	if (game_state == STOP_STATE)
	{
		go_manager->SaveSceneOnPlay();
	}

	game_state = PLAY_STATE;
	game_timer.PlayGame();

}

void Application::Stop()
{
	game_state = STOP_STATE;
	game_timer.StopGame();
	go_manager->LoadSceneOnStop();
}

void Application::Pause()
{
	game_state = PAUSED_STATE;
	game_timer.PauseGame();
}

GAME_STATE Application::GetGameState()
{
	return game_state;
}

double Application::GetDeltaTime()
{
	return game_timer.game_time;
}


bool Application::LoadConfig()
{
	bool ret = true;

	//LoadData from Config
	JSON_Value* config = json_parse_file("config.json");

	assert(config != nullptr);

	//Geting App data
	JSON_Object* data = json_value_get_object(config);
	JSON_Object* app_data = json_object_get_object(data, "App");

	// Call LoadConfig() in all modules
	std::list<Module*>::const_iterator it = list_modules.begin();
	while (it != list_modules.end())
	{
		JSON_Object* module_config = json_object_get_object(app_data, (*it)->name.c_str());
		ret = (*it)->LoadConfig(module_config);
		++it;
	}

	json_value_free(config);

	return ret;
}

bool Application::SaveConfig()
{
	bool ret = true;

	JSON_Value* file = json_parse_file("config.json");
	JSON_Object* config = json_value_get_object(file);
	JSON_Object* app_config = json_object_get_object(config, "App");

	json_object_set_string(config, "name", name.c_str());
	json_object_set_string(config, "organization", organization.c_str());

	// Call SaceConfig() in all modules
	std::list<Module*>::const_iterator it = list_modules.begin();
	while (it != list_modules.end())
	{
		JSON_Object* module_config = json_object_get_object(app_config,(*it)->name.c_str());
		ret = (*it)->SaveConfig(module_config);
		++it;
	}

	json_serialize_to_file_pretty(file, "config.json");
	json_value_free(file);

	return ret;
}