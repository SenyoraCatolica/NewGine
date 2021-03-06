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
	MeshComponent(MeshComponent& m);
	~MeshComponent();

	void Update();

	void ComponentEditor();

	void SetResourceMesh(ResourceMesh * resourse_mesh);

	void RecalculateLocalbox();
	void RecalculateBox();
	math::AABB GetGlobalBox();
	void SetBBActive(bool active);

	void Save(JSONWrapper& file) const;
	void Load(JSONWrapper& file);

public:
	
	ResourceMesh* mesh = nullptr;
	math::AABB local_box;
	math::AABB global_box;
	bool bb_active = false;
};
