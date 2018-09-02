
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
	std::list<GameObject*>::const_iterator it = all_gameobjects.begin();
	while (it != all_gameobjects.end())
	{
		//TODO GameObjects Update should be called here
		App->renderer3D->DrawMesh((*it)->GetMesh());
		it++;
	}

	return update_status::UPDATE_CONTINUE;
}

