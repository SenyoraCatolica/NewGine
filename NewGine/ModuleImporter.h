#pragma once

#include "Module.h"
#include "GameObject.h"
#include "ModuleResourceManager.h"
#include "MyResource.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"


#pragma comment (lib,"Assimp/libx86/assimp.lib")


class ModuleImporter : public Module
{
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Init(JSON_Object* node);
	bool Start();
	update_status PreUpdate(float dt);
	bool CleanUp();


	bool Import(const char* file, MyResource::R_STATE state);

public:
	MeshImport * iMesh = nullptr;
	MaterialImport* iMaterial = nullptr;

};