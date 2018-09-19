#pragma once

#include "Module.h"
#include "Globals.h"
#include "MyResource.h"
#include <map>

class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager(Application* app, bool start_enabled = true);
	~ModuleResourceManager();

	bool Start();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

public:
	MyResource* CreateResource(MyResource::R_TYPE, uint uuid);
	MyResource* GetResource(uint uuid);
	MyResource* TryGetResourceByName(const char* name);



public:
	std::map<uint, MyResource*> resources;

};
