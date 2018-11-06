#include "ResourceMaterial.h"
#include "ResourceMesh.h"
#include "Application.h"


ModuleResourceManager::ModuleResourceManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Resource Manager";
}

ModuleResourceManager::~ModuleResourceManager()
{
	resources.clear();
}

bool ModuleResourceManager::Init()
{
	//2DO:Loading resources
	return true;
}

bool ModuleResourceManager::Start()
{
	App->file_system->CreateDir("Library");
	App->file_system->CreateDir("Library/Meshes");
	App->file_system->CreateDir("Library/Textures");
	App->file_system->CreateDir("Library/Materials");
	App->file_system->CreateDir("Library/Meta");
	App->file_system->CreateDir("Assets/Scenes");

	return true;
}

/*update_status ModuleResourceManager::PreUpdate(float dt)
{
	//2DO import possible dropped files

	//2DO Add all the files to reimport to a new dicctionary

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleResourceManager::Update(float dt)
{
	return update_status::UPDATE_CONTINUE;
}


update_status ModuleResourceManager::PostUpdate(float dt)
{
	//2DO delete non used resources

	//2Do delete all resources to delete
	return update_status::UPDATE_CONTINUE;
}*/

bool ModuleResourceManager::CleanUp()
{
	//2Do save the data
	return true;
}

MyResource* ModuleResourceManager::CreateResource(MyResource::R_TYPE type, uint uuid)
{
	MyResource* ret = nullptr;
	uint uid;

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
	for (uint i = 0; i < resources.size(); i++)
	{
		if (strcmp(it->second->name, name) == 0)
		{
			return it->second;
		}
		it++;
	}
	return nullptr;
}

FILE_TYPE TryGetTypeByName(const char* file)
{
	FILE_TYPE ret = NONE;

	if (file != nullptr)
	{
		char* mesh_extensions[] = { "fbx", "FBX", "obj", "OBJ" };
		char* material_extensions[] = { "png", "PNG", "tga", "TGA" };

		std::string name = file;
		std::string extension = name.substr(name.find_last_of(".") + 1);


		for (int i = 0; i < 4; i++)
			if (extension.compare(mesh_extensions[i]) == 0)
				return ret = MESH;

		for (int i = 0; i < 4; i++)
			if (extension.compare(material_extensions[i]) == 0)
				return MATERIAL;
	}

	return ret;
}


void ModuleResourceManager::ImportFile(const char* file)
{
	FILE_TYPE type = TryGetTypeByName(file);

	if (type != NONE)
	{
		App->importer->Import(file, type);
		//2DO Update assets window
	}

	else
	{
		LOG("Could not determine the type of the file %s:", file);
	}
}
