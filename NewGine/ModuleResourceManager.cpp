#include "ResourceMaterial.h"
#include "ResourceMesh.h"
#include "Application.h"
#include "GlobalFunctions.h"



ModuleResourceManager::ModuleResourceManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Resource Manager";
}

ModuleResourceManager::~ModuleResourceManager()
{
	std::map<uint, MyResource*>::iterator it = resources.begin();
	/*while (it != resources.end())
	{
		delete &it;
		it++;
	}*/

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
	MyResource* ret = nullptr;

	std::map<uint, MyResource*>::iterator it = resources.begin();
	while (it != resources.end())
	{
		if ((*it).first == uuid)
			ret = (*it).second;

		it++;
	}

	return ret;
}

MyResource* ModuleResourceManager::TryGetResourceByName(const char* name)
{
	std::map<uint, MyResource*>::iterator it = resources.begin();
	for (uint i = 0; i < resources.size(); i++)
	{
		if (it->second->name == name)
		{
			return GetResource(it->first);
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
		//if (App->file_system->Exists(file))
			App->importer->Import(file, type);

		//else
		//	ImportFromOutsideFolder(file, type);
		//2DO Update assets window
	}

	else
	{
		LOG("Could not determine the type of the file %s:", file);
	}
}

void ModuleResourceManager::ImportFromOutsideFolder(const char* file, FILE_TYPE type)
{
	string dir = file;

	//Assets 
	string assets_dir = App->editor->GetAssetsWindow()->GetAssetsDirectory() + App->file_system->GetNameFromDirectory(file);
	if(App->file_system->Exists(App->editor->GetAssetsWindow()->GetAssetsDirectory()) == false)
		App->file_system->CreateDir(ASSETS_FOLDER);

	//Library
	string library_dir;

	switch (type)
	{
	case MESH:
		library_dir = MESH_FOLDER;
		break;
	case MATERIAL:
		library_dir = MATERIAL_FOLDER;
		break;
	case NONE:
		LOG("Could not determine the type of the file %s:", file);
		return;
		break;
	default:
		break;
	}

	if(App->file_system->Exists(library_dir.data()) == false)
		App->file_system->CreateDir(library_dir.data());

	//Create the data to transform it into an own file
	uint uuid = GenerateUUID();

	string complete_path_assets = assets_dir;
	complete_path_assets = complete_path_assets.substr(0, complete_path_assets.length() - 4);

	CreateFileMeta(uuid, type, library_dir.data(), complete_path_assets.data());

	library_dir += "/";	

	App->importer->Import(complete_path_assets.data(), type);
}

void ModuleResourceManager::CreateFileMeta(uint uuid, FILE_TYPE type, const char* path, const char* lib_dir)
{
	JSONWrapper root;

	root.WriteUInt("UUID", uuid);
	root.WriteUInt("Type", (int)type);
	root.WriteString("LibraryPath", lib_dir);
	root.WriteString("OriginalPath", path);
	

	char* buff;
	size_t size = root.SerializeBuffer(&buff);  //get the size of the buffer

	string final_path = path;
	final_path = final_path.substr(0, final_path.length() - 4);
	final_path += ".meta";

	App->file_system->Save(final_path.data(), buff, size);

	delete[] buff;
}

std::string ModuleResourceManager::CopyFileToAssets(const char* path, std::string assets_dir)
{
	string dir;

	if (assets_dir.empty())
	{
		dir = App->editor->GetAssetsWindow()->GetAssetsDirectory();
	}

	else
	{
		dir = assets_dir;
	}

	dir += App->file_system->GetNameFromDirectory(path);
	App->file_system->CopyFileToDir(path, dir.data());

	return dir;
}

ResourceMesh* ModuleResourceManager::LinkResourceMesh(const char* name, const char* path)
{
	ResourceMesh* m = (ResourceMesh*)TryGetResourceByName(name);

	if (m == nullptr)
	{
		m = (ResourceMesh*)LoadResource(path, MESH);
	}
	
	if (m)
		return m;
	return nullptr;
}

ResourceMaterial* ModuleResourceManager::LinkResourceMaterial(const char* name, const char* path)
{
	ResourceMaterial* m = (ResourceMaterial*)TryGetResourceByName(name);

	if (m == nullptr)
	{
		m = (ResourceMaterial*)LoadResource(path, MATERIAL);
	}

	if (m)
		return m;
	return nullptr;
}

MyResource* ModuleResourceManager::LoadResource(const char* path, FILE_TYPE type)
{
	MyResource* ret = nullptr;

	switch (type)
	{
	case MESH:

		ret = new ResourceMesh(GenerateUUID(), path);
		break;
	case MATERIAL:
		ret = new ResourceMaterial(GenerateUUID(), path);
		break;
	case NONE:
		LOG("Could not load the resource: TYPE == NONE");
		break;
	default:
		break;
	}

	return ret;
}


uint ModuleResourceManager::GetUUIDFromResourcePath(const char* path)
{
	uint ret = 0;

	string p = path;
	p.pop_back();

	int last = p.find_last_of("/\\") + 1;
	string name = p.substr(last);
	name = name.substr(0, name.find_last_of('.'));

	if (name.length() == 0)
		ret = 0;
	else
		ret = std::stoul(name);

	return ret;
}




