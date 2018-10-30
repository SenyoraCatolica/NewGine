
#include"ModuleGOManager.h"
#include "GameObject.h"
#include "Application.h"
#include "CameraComponent.h"
#include "MeshComponent.h"


ModuleGOManager::ModuleGOManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	quadtree = new Quadtree(float3(WORLD_WIDTH / -2, WORLD_HEIGHT / -2, WORLD_DEPTH / -2), float3(WORLD_WIDTH / 2, WORLD_HEIGHT / 2, WORLD_DEPTH / 2));
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

GameObject* ModuleGOManager::CreateEmpty(const char* name)
{
	GameObject* new_go = new GameObject();
	new_go->AddComponent(COMPONENT_TRANSFORM);

	if (name != nullptr && name != "")
	{
		new_go->SetName(name);
	}

	new_go->parent = root;

	return new_go;
}

GameObject* ModuleGOManager::CreateGameObject(const char* name, GameObject* parent)
{
	GameObject* new_go = new GameObject();
	new_go->AddComponent(COMPONENT_TRANSFORM);

	if (name != nullptr && name != "")
	{
		new_go->SetName(name);
	}

	if (parent != nullptr)
		new_go->parent = parent;
	else
		new_go->parent = root;

	return new_go;
}

bool ModuleGOManager::DeleteGameObject(GameObject* to_delete)
{
	bool ret = false;

	if (to_delete)
	{
		to_delete_gos.push_back(to_delete);
		ret = true;
	}

	return ret;
}

GameObject* ModuleGOManager::CreateCamera(const char* name)
{
	GameObject* cam = CreateEmpty("Camera");
	cam->AddComponent(COMPONENT_CAMERA);

	return cam;
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

	return nullptr;
}

CameraComponent* ModuleGOManager::GetCameraComponent()
{
	return (CameraComponent*)GetCameraObject()->GetComponent(COMPONENT_CAMERA);
}

GameObject* ModuleGOManager::Raycast(const Ray& ray)const
{
	GameObject* ret = nullptr;


	vector<GameObject*> cols;   //list og game objects that collided with the ray

	cols = quadtree->GetColliderObjects(ray);

	std::list<GameObject*>::iterator it = App->go_manager->dynamic_objects.begin();
	while (it != dynamic_objects.end())
	{
		if ((*it)->IsActive() && (*it)->aabb.IsFinite() && (*it)->aabb.Intersects(ray) == true)
		{
			cols.push_back((*it));
		}

		it++;
	}

	return ret;
}




