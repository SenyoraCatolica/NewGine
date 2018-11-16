#pragma once


#include "Module.h"
#include "Globals.h"
#include "MyResource.h"
#include <map>

#include "MeshComponent.h"
#include "MaterialComponent.h"
#include "ResourceMesh.h"
#include "ResourceMaterial.h"

enum FILE_TYPE
{
	MESH, MATERIAL, NONE
};

class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager(Application* app, bool start_enabled = true);
	~ModuleResourceManager();

	bool Init();
	bool Start();
	bool CleanUp();

	MyResource* CreateResource(MyResource::R_TYPE, uint uuid);
	MyResource* GetResource(uint uuid);
	MyResource* TryGetResourceByName(const char* file);
	ResourceMesh* CopyResourceMesh(ResourceMesh* copy);
	ResourceMaterial* CopyResourceMaterial(ResourceMaterial* copy);



	//Handle files
	void ImportFile(const char* file);
	void ImportFromOutsideFolder(const char* file, FILE_TYPE type);
	void CreateFileMeta(uint uuid, FILE_TYPE type, const char* lib_dir, const char* assets_dir);
	std::string CopyFileToAssets(const char* path, std::string assets_dir);

	ResourceMesh* LinkResourceMesh(const char* name, const char* path);
	ResourceMesh* LinkResourceMeshByName(const char* name);
	ResourceMesh* LinkResourceMeshByPath(const char* path);

	ResourceMaterial* LinkResourceMaterial(const char* name, const char* path);
	ResourceMaterial* LinkResourceMaterialByName(const char* name);
	ResourceMaterial* LinkResourceMaterialByPath(const char* path);
	MyResource* LoadResource(const char* path, FILE_TYPE type);

	uint GetUUIDFromResourcePath(const char* path);

public:
	std::map<uint, MyResource*> resources;

};
