#pragma once

#include "Application.h"
#include "GameObject.h"

struct aiMesh;
struct aiNode;
struct aiScene;
struct MyMesh;

class MeshImporter
{
public:
	MeshImporter(Application* app);
	~MeshImporter();

	bool Import(const char* file);
	GameObject* ImportNode(aiNode* node, const aiScene* scene, GameObject* parent, const char* save_path, JSONWrapper& root_node);
	bool ImportMesh(const aiScene * scene, const aiMesh* mesh, GameObject* go, const char* name, const char* save_path, JSONWrapper& root, uint uuid = 0);
	MyMesh* LoadMesh(const char* path);
	bool SaveMesh(MyMesh* m, const char* name, const char* save_path);

	void DecomposeTransform(TransformComponent* trans, aiNode* node);

private:
	Application* App;

};
