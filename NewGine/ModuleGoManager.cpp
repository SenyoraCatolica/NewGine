
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
	selected_go = nullptr;
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

	SelectObject();

	//2DO condition to draw
	App->renderer3D->DebugDrawQuadtree(quadtree, quadtree->GetRoot());

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

	//Sort all the candidates by distance
	std::map<float, GameObject*> candidates;
	std::list<GameObject*>::iterator it = App->go_manager->dynamic_objects.begin();
	while (it != dynamic_objects.end())
	{
		float near_dist, far_dist;
		if ((*it)->obb.Intersects(ray, near_dist, far_dist) == true)
		{
			candidates.insert(std::pair<float, GameObject*>(MIN(near_dist, far_dist), (*it)));
		}
	}


	//iterate all the possible collisions by order
	std::map<float, GameObject*>::iterator mapit = candidates.begin();

	while (mapit != candidates.end())
	{
		float coldist = 100000;

		//check if go has mesh
		if (mapit->second->HasComponent(COMPONENT_MESH))
		{
			TransformComponent* t = (TransformComponent*)mapit->second->GetComponent(COMPONENT_TRANSFORM);

			Ray transposed_ray = ray;
			transposed_ray.Transform(t->GetGlobalTranform().InverseTransposed());

			MeshComponent* m = (MeshComponent*)mapit->second->GetComponent(COMPONENT_MESH);
			const uint num_indices = m->mesh->mesh->num_indices;

			uint u1, u2, u3;
			float3 v1, v2, v3;
			Triangle triangle;
			float distance;
			float3 hitpoint;


			//now create triangles and check them
			for (uint i = 0; i < num_indices/3; i++)
			{
				u1 = m->mesh->mesh->indices[i * 3];
				u2 = m->mesh->mesh->indices[i * 3 + 1];
				u3 = m->mesh->mesh->indices[i * 3 + 2];
				v1 = float3(&m->mesh->mesh->vertices[u1]);
				v2 = float3(&m->mesh->mesh->vertices[u2]);
				v3 = float3(&m->mesh->mesh->vertices[u3]);
				triangle = Triangle(v1, v2, v3);

				if (triangle.Intersects(transposed_ray, &distance, &hitpoint) == true)
				{
					if (distance < coldist)
					{
						//save the mesh with less dist
						ret = mapit->second;
					}
				}
			}
		}
	}


	return ret;
}


void ModuleGOManager::SelectObject()
{
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_UP)
	{
		CameraComponent* cam = (CameraComponent*)camera->GetComponent(COMPONENT_CAMERA);

		float2 pos(App->input->GetMouseX(), App->input->GetMouseY());

		pos.x = 2.0f * pos.x / (float)App->window->GetWidth() - 1.0f;
		pos.y = 1.0f - 2.0f * pos.y / (float)App->window->GetHeight();

		Ray ray = cam->frustum.UnProjectFromNearPlane(pos.x, pos.y);

		selected_go = Raycast(ray);
	}
}


