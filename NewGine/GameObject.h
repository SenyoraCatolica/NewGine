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
	GameObject(const uint64_t uid);
	~GameObject();

	void InitBuffer();

	//2DO this will be deprecated
	MyMesh GetMesh();
	void SetMesh(MyMesh m);

	const uint64_t GetUID();
	void DrawLocator();
	void DrawAABB();
	void DrawOBB();

	void Select();
	void Unselect();

	void SetOriginalAABB();
	void UpdateAABB();

	void UpdateTransformMatrix();

	void SetActive(bool state);
	bool IsActive();

	void SetStatic(bool Stat);
	bool IsStatic();

	void SetName(const char* newName);
	const char* GetName();



	Component* AddComponent(COMPONENT_TYPE type, uint id_num);
	bool DeleteComponent(Component* ComponentToDelete);

	bool HasComponent(COMPONENT_TYPE type);

	template <typename typeComp>
	std::vector<typeComp*> GetComponent()
	{
		std::vector<typeComp*> ret;
		if (HasComponent(typeComp::GetType()))
		{
			std::vector<Component*>::iterator it = components.begin();
			while (it != components.end())
			{
				if ((*it)->GetType() == typeComp::GetType())
				{
					ret.push_back((typeComp*)(*it));
				}
				it++;
			}
		}
		return ret;
	}


private:
	uint64_t uid;
	bool active = true;
	bool is_static = false;
	bool selected = false;

	AABB originalAABB;
	TransformComponent* transform = nullptr;



public:

	char name[NAME_MAX_LEN];
	AABB aabb;
	OBB obb;

	MyMesh mesh;
	GameObject* parent = nullptr;

	std::vector<Component*> childs;
	std::list<Component*> components;
};
