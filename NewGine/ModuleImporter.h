#pragma once

#include "Module.h"
#include "GameObject.h"
#include "ModuleResourceManager.h"
#include "MyResource.h"
#include <string>

#include "MeshImporter.h"
#include "MaterialImporter.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"


#pragma comment (lib,"Assimp/libx86/assimp.lib")


class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	bool CleanUp();


	bool Import(const char* file, FILE_TYPE state);

public:
	MeshImporter * mesh_importer = nullptr;
	MaterialImporter* mat_importer = nullptr;

};