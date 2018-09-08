#pragma once
#include"Globals.h"

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
	virtual void Update(float dt) {};
	virtual void Disable();
	virtual void ComponentEditor() {};

	GameObject* GetObjectParent();


public:

	COMPONENT_TYPE type;
	uint id;
	bool enabled = true;

	GameObject* parent;
};