#ifndef COMPONENT_H
#define COMPONENT_H

#include "Globals.h"
#include "MathGeoLib\include\MathGeoLib.h"




class GameObject;

enum component_type
{
	COMPONENT_TRANSFORM,
	COMPONENT_MESH,
	COMPONENT_MATERIAL
};

class Component
{
public:
	Component();
	Component(component_type Type,  GameObject* from, uint number);
	Component(component_type Type, GameObject* from);
	~Component();

	virtual void Enable();
	virtual void Update(float dt) {};
	virtual void Disable();
	virtual void ComponentEditor() {};

	virtual math::float4x4 GetLocalTransform() { return math::float4x4::identity; };
	virtual math::float4x4 GetGlobalTranform() { return math::float4x4::identity; };
	virtual void SetGlobalTransform(math::float4x4 trans) { };

public:

	component_type type;
	uint id;
	bool enabled = true;
	GameObject* parent;
};
#endif // COMPONENT_H

