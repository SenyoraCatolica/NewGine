#pragma once

#include "Globals.h"
#include <vector>
#include <list>
#include "MathGeoLib\include\MathGeoLib.h"
#include "Component.h"



struct MyTexture
{
	uint id;
	std::string path;
	uint width, height;
};

struct MyMesh
{
	//Vertices
	uint id_vertices = 0;
	uint num_vertices = 0;
	float* vertices = nullptr;

	//Indices
	uint id_indices = 0;
	uint num_indices = 0;
	uint* indices = nullptr;

	//Normals
	uint id_normals = 0;
	uint num_normals = 0;
	float* normals = nullptr;

	//Texture
	uint id_texture_coords = 0;
	uint num_texture_coords = 0;
	float2* texture_coords;
	std::vector<MyTexture*> textures;

	float3 pos, rot, scale;
};

class GameObject
{
public:

	GameObject();
	~GameObject();

	void InitBuffer();

	//2DO this will be deprecated
	MyMesh GetMesh();
	void SetMesh(MyMesh m);

	Component* AddComponent(COMPONENT_TYPE type, uint id_num);
	bool DeleteComponent(Component* ComponentToDelete);

	bool HasComponent(COMPONENT_TYPE type);
	Component* GetComponent(COMPONENT_TYPE type);


private:
	bool wireframe;

public:
	MyMesh mesh;

	std::list<Component*> components;
};
