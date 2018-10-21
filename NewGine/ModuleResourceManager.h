#pragma once


#include "Module.h"
#include "Globals.h"
#include "MyResource.h"
#include <map>

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
	bool CleanUp();

	MyResource* CreateResource(MyResource::R_TYPE, uint uuid);
	MyResource* GetResource(uint uuid);
	MyResource* TryGetResourceByName(const char* file);
	//FILE_TYPE TryGetTypeByName(const char* file);

	//Handle files
	void ImportFile(const char* file);

public:
	std::map<uint, MyResource*> resources;

};
