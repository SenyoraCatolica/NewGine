#pragma once

#include "Application.h"
#include "GameObject.h"

struct MyMesh;

class MeshImporter
{
public:
	MeshImporter(Application* app);
	~MeshImporter();

	bool Import(const char* file);
	GameObject* ImportNode(aiNode* node, const aiScene* scene, GameObject* parent);
	bool ImportMesh(const aiScene * scene, const aiMesh* mesh, GameObject* go, const char* name, uint uuid = 0);
	MyMesh* LoadMesh(const char* path);
	bool SaveMesh(MyMesh* m, const char* name);

	void DecomposeTransform(TransformComponent* trans, aiNode* node);

private:
	Application* App;

};
