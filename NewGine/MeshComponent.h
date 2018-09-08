#pragma once

#include "Globals.h"
#include "Component.h"
#include "GameObject.h"
#include "MathGeoLib\include\MathGeoLib.h"


struct MyMesh;

class MeshComponent : public Component
{
public:
	MeshComponent(GameObject* game_object);
	MeshComponent(COMPONENT_TYPE type, GameObject* game_object);
	~MeshComponent();

	void Update(float dt);

	void ComponentEditor();
	void RecalculateBox();
	math::AABB GetGlobalBox();


public:
	MyMesh * mesh = NULL;

	math::AABB local_box;
	math::AABB global_box;
};
