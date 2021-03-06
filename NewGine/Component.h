#pragma once
#include"Globals.h"
#include "JSONWrapper.h"

enum COMPONENT_TYPE
{
	COMPONENT_TRANSFORM,
	COMPONENT_MESH,
	COMPONENT_MATERIAL,
	COMPONENT_CAMERA
};

class GameObject;

class Component
{
public:
	
	Component();
	Component(COMPONENT_TYPE type, GameObject* from);
	Component(COMPONENT_TYPE type, GameObject* from, uint id);
	~Component();

	virtual void Enable();
	virtual void Update() {};
	virtual void Disable();
	virtual void ComponentEditor() {};

	GameObject* GetObjectParent();

	virtual void Save(JSONWrapper& file) const;
	virtual void Load(JSONWrapper& file);

public:

	COMPONENT_TYPE type;
	uint id = 0;
	bool enabled = true;
	std::string path;
	GameObject* parent;
};