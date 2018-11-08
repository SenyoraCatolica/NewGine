#pragma once

#include "MyResource.h"
#include "MathGeoLib\include\MathGeoLib.h"

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

	//Texture Coords
	uint id_texture_coords = 0;
	uint num_texture_coords = 0;
	float2* texture_coords;

};

class ResourceMesh : public MyResource
{
public:
	ResourceMesh(uint uid);
	ResourceMesh(uint uid, const char* path);
	virtual ~ResourceMesh();

	void GenerateResource(MyMesh* m);

	bool LoadToMemory();
	void DeleteToMemory();


public:
	bool hasNormals = false;
	MyMesh* mesh = nullptr;

};

