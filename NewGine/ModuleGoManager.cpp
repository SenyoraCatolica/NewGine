
#include"ModuleGOManager.h"
#include "GameObject.h"
#include "Application.h"

ModuleGOManager::ModuleGOManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleGOManager::~ModuleGOManager()
{

}

bool ModuleGOManager::Init()
{

	return true;
}


update_status ModuleGOManager::Update(float dt)
{
	return update_status::UPDATE_CONTINUE;
}

