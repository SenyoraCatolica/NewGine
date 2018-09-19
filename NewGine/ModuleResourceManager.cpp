#include "ModuleResourceManager.h"
#include "Application.h"
#include "ResourceMaterial.h"
#include "ResourceMesh.h"


ModuleResourceManager::ModuleResourceManager(Application* app, bool start_enabled = true) : Module(app, start_enabled)
{
	name = "Resource Manager";
}

ModuleResourceManager::~ModuleResourceManager()
{
	resources.clear();
}

bool ModuleResourceManager::Start()
{
	//2DO:Loading resources
}
update_status ModuleResourceManager::PreUpdate(float dt)
{
	//2DO import possible dropped files

	//2DO Add all the files to reimport to a new dicctionary

}
update_status ModuleResourceManager::PostUpdate(float dt)
{
	//2DO delete non used resources

	//2Do delete all resources to delete
}

bool ModuleResourceManager::CleanUp()
{
	//2Do save the data
}

MyResource* ModuleResourceManager::CreateResource(MyResource::R_TYPE type, uint uuid)
{
	MyResource* ret = nullptr;
	uint64_t uid;

	if (uuid == 0)
		uid = GenerateUUID();
	else
		uid = uuid;

	switch (type)
	{
		case MyResource::MATERIAL: 
			ret = (MyResource*) new ResourceMaterial(uid);
			break;

		case MyResource::MESH: 
			ret = (MyResource*) new ResourceMesh(uid);
			break;
	}

	if (ret != nullptr)
		resources[uid] = ret;

	return ret;
}

MyResource* ModuleResourceManager::GetResource(uint uuid)
{
	std::map<uint, MyResource*>::iterator it = resources.find(uuid);
	if (it != resources.end())
		return it->second;
	return nullptr;
}

MyResource* ModuleResourceManager::TryGetResourceByName(const char* name)
{
	std::map<uint, MyResource*>::iterator it = resources.begin();
	for (int i = 0; i < resources.size(); i++)
	{
		if (strcmp(it->second->name, name) == 0)
		{
			return it->second;
		}
		it++;
	}
	return nullptr;
}