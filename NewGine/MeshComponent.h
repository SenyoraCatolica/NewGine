#pragma once

#include "Globals.h"
#include "Component.h"
#include "GameObject.h"
#include "MaterialComponent.h"
#include "MathGeoLib\include\MathGeoLib.h"



class MeshComponent : public Component
{
public:
	MeshComponent(COMPONENT_TYPE type, GameObject* game_object);
	~MeshComponent();

	void Update(float dt);

	void ComponentEditor();

	void SetResourceMesh(ResourceMesh * resourse_mesh);


public:
	
	ResourceMesh* mesh;
	math::AABB local_box;
	math::AABB global_box;
};
