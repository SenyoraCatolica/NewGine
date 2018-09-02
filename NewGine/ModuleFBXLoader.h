#pragma once

#include "Globals.h"
#include "Module.h"
#include "GameObject.h"
#include <vector>
#include "Assimp/include/scene.h"
#include "Assimp/include/cfileio.h"




class ModuleFBXLoader : public Module
{
public:

	ModuleFBXLoader(Application* app, bool start_enabled = true);
	~ModuleFBXLoader();

	bool Init();

	void LoadFBX(const char* filename, const char* texpath = nullptr);
	MyMesh LoadMesh(const aiMesh* mesh, const aiScene* scene, const char* texpath = nullptr);

	uint LoadTexture(const char* filename);

private:

	GameObject * go;
	const char* current_filename;
};
