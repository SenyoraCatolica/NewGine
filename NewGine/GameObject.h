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

	MyMesh GetMesh();
	void SetMesh(MyMesh m);

private:
	bool wireframe;

public:
	MyMesh mesh;

	list<Component*> components;
};
