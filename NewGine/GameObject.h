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
	void Select();
	void Unselect();

	void Update();
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

private:
	uint32 uid;
	bool active = true;
	bool is_static = false;

public:
	char name[NAME_MAX_LEN];
	AABB aabb;

	bool boundingbox_active = false;
	bool selected = false;

	GameObject* parent = nullptr;


	std::vector<GameObject*> childs;
	std::list<Component*> components;
};
