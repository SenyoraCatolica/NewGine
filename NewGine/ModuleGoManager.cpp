
#include"ModuleGOManager.h"
#include "GameObject.h"
#include "Application.h"
#include "CameraComponent.h"
#include "MeshComponent.h"


ModuleGOManager::ModuleGOManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleGOManager::~ModuleGOManager()
{
	delete camera;
	delete cam_comp;

	camera = nullptr;
	cam_comp = nullptr;
}

bool ModuleGOManager::Init()
{
	return true;
}


update_status ModuleGOManager::Update(float dt)
{
	objects_to_draw.clear();

	std::list<GameObject*>::iterator it = all_gameobjects.begin();

	while (it != all_gameobjects.end())
	{
		MeshComponent* mesh = (MeshComponent*)(*it)->GetComponent(COMPONENT_MESH);
		if (mesh)
		{
			if (GetCameraComponent()->frustum.Intersects(mesh->GetGlobalBox()))
			{
				objects_to_draw.push_back(*it);
			}
		}
		it++;
	}

	it = objects_to_draw.begin();
	while (it != objects_to_draw.end())
	{
		//2DO draw game objects to draw
		it++;
	}


	return update_status::UPDATE_CONTINUE;
}

GameObject* ModuleGOManager::GetCameraObject()
{
	std::list<GameObject*>::iterator it = App->go_manager->all_gameobjects.begin();

	while (it != App->go_manager->all_gameobjects.end())
	{
		if ((*it)->GetComponent(COMPONENT_CAMERA) != nullptr)
		{
			return (*it);
		}
		it++;
	}
}

CameraComponent* ModuleGOManager::GetCameraComponent()
{
	return (CameraComponent*)GetCameraObject()->GetComponent(COMPONENT_CAMERA);
}



