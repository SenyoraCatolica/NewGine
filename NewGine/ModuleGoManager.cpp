
#include"ModuleGOManager.h"
#include "GameObject.h"
#include "Application.h"

ModuleGOManager::ModuleGOManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleGOManager::~ModuleGOManager()
{
	if (root != NULL)
		delete root;
}

bool ModuleGOManager::Start()
{
	GameObject* root_object = App->go_manager->CreateGameObject();
	root_object->name = "Root GameObject";
	App->go_manager->root = root_object;

	return true;
}

GameObject* ModuleGOManager::CreateGameObject(GameObject* parent)
{
	GameObject* tmp = new GameObject(parent);

	if (parent == NULL)
	{
		root = tmp;
	}

	else
		parent->AddChild(tmp);

	all_gameobjects.push_back(tmp);

	return tmp;
}

GameObject* ModuleGOManager::LoadGameObject(const char* path)
{
	GameObject* ret = nullptr;
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene)
	{
		ret = App->load->LoadNode(scene->mRootNode, scene, App->go_manager->root);
	}

	aiReleaseImport(scene);
	scene = nullptr;
	return ret;
}

void ModuleGOManager::Delete(GameObject* GO_to_delete)
{
	GameObject* tmp = GO_to_delete;

	//Find last child
	while (tmp->childs.empty())
		tmp = *tmp->childs.begin();

	//Delete children to top
	while (tmp != GO_to_delete)
	{
		tmp = tmp->parent;

		//erase childs
		list<GameObject*>::const_iterator it = GO_to_delete->childs.begin();
		while (it != GO_to_delete->childs.end())
		{
			delete *it;
		}

		tmp->childs.clear();
	}

	delete GO_to_delete;
}

update_status ModuleGOManager::Update(float dt)
{

	TransformationHierarchy(root);

	list<GameObject*>::const_iterator it = all_gameobjects.begin();
	while (it != all_gameobjects.end())
	{
		(*it)->Update(dt);
		App->renderer3D->DrawMesh((MeshComponent*)(*it)->mesh, (TransfomComponent*)(*it)->transform, (MaterialComponent*)(*it)->material);
		it++;
	}
	return update_status::UPDATE_CONTINUE;
}

GameObject* ModuleGOManager::GetRoot()
{
	return root;
}
void ModuleGOManager::TransformationHierarchy(GameObject* object)
{
	if (root->childs.empty() == false)
	{
		if (object == *root->childs.begin())
			object->GetComponent(COMPONENT_TRANSFORM)->SetGlobalTransform(object->GetComponent(COMPONENT_TRANSFORM)->GetLocalTransform());

		if (object->GetComponent(COMPONENT_TRANSFORM) != nullptr)
		{
			if (object->parent->GetComponent(COMPONENT_TRANSFORM) != nullptr)
			{
				object->GetComponent(COMPONENT_TRANSFORM)->SetGlobalTransform(object->parent->GetComponent(COMPONENT_TRANSFORM)->GetGlobalTranform() * object->GetComponent(COMPONENT_TRANSFORM)->GetLocalTransform());
			}

			else
				object->GetComponent(COMPONENT_TRANSFORM)->SetGlobalTransform(object->GetComponent(COMPONENT_TRANSFORM)->GetLocalTransform());
		}

		if (object->childs.size())
		{
			list<GameObject*>::iterator it = object->childs.begin();

			while (it != object->childs.end())
			{
				TransformationHierarchy((*it));
				it++;
			}
		}
	}
	

}

