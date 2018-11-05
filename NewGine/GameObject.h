#pragma once

#include "Globals.h"
#include <vector>
#include <list>
#include "MathGeoLib\include\MathGeoLib.h"
#include "Component.h"
#include "TransformComponent.h"
#include "ResourceMesh.h"



class GameObject
{
public:

	GameObject();
	GameObject(const uint uid);
	GameObject(const char* name, uint uuid, GameObject* parent, bool is_static, bool is_active);
	~GameObject();

	const uint64_t GetUID();
	void DrawLocator();
	void DrawAABB();
	void DrawOBB();

	void Select();
	void Unselect();

	void UpdateAABB();

	void UpdateTransformMatrix();

	void SetActive(bool state);
	bool IsActive();

	void SetStatic(bool Stat);
	bool IsStatic();

	void SetName(const char* newName);
	const char* GetName();

	void Save(JSONWrapper& file);

	//Components
	Component* AddComponent(COMPONENT_TYPE type);
	bool DeleteComponent(Component* ComponentToDelete);

	bool HasComponent(COMPONENT_TYPE type);
	Component* GetComponent(COMPONENT_TYPE type);


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
	uint uid;
	bool active = true;
	bool is_static = false;

	AABB originalAABB;


public:
	char name[NAME_MAX_LEN];
	AABB aabb;
	OBB obb;
	bool boundingbox_active = false;
	bool selected = false;

	GameObject* parent = nullptr;


	std::vector<GameObject*> childs;
	std::list<Component*> components;
};
