#pragma once

#include "Application.h"
#include "GameObject.h"

struct MyMesh;

class MeshImporter
{
	MeshImporter(Application* app);
	~MeshImporter();

	bool Import(const aiScene * scene, const aiMesh* mesh, GameObject* go, const char* name, uint uuid = 0);

private:
	Application* App;

};
