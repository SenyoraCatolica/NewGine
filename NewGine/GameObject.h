#pragma once

#include "Globals.h"
#include <vector>



struct MyMesh
{
	// VERTICES ---------------------
	uint id_vertices = 0;
	uint num_vertices = 0;
	float* vertices = nullptr;

	// INDICES -----------------
	uint id_indices = 0;
	uint num_indices = 0;
	uint* indices = nullptr;

	// TEXTUTE COORDS -----------------
	uint id_image = 0;
	std::string tex_name;
	std::string tex_path;

	uint id_texture = 0;
	float* tex_coords = nullptr;

	//NORMALS COORDS --------------
	uint id_normals = 0;
	float* normals = nullptr;

	/*~MyMesh()
	{
	delete[] indices;
	delete[] vertices;
	delete[] tex_coords;
	delete[] normals;
	}*/
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
};
