#pragma once

#include "Globals.h"
#include "Component.h"
#include "GameObject.h"
#include "ResourceMaterial.h"

#include <vector>

class ResourceMaterial;

class MaterialComponent : public Component
{
public:
	MaterialComponent(COMPONENT_TYPE type, GameObject* game_object);
	~MaterialComponent();

	void ComponentEditor();

	uint GetTexture(uint n);
	void SetColor(float r, float g, float b, float a = 1.0f);

	void SetResourceMaterial(ResourceMaterial* mat);

	void Save(JSONWrapper& file) const;
	void Load(JSONWrapper& file);

public:

	ResourceMaterial* material = nullptr;
	std::string path;

	float color[4] = { 1.0f,1.0f,1.0f,1.0f };
};


