#pragma once

#include "Globals.h"
#include "Component.h"
#include "GameObject.h"

#include <vector>



class MaterialComponent : public Component
{
public:
	MaterialComponent(COMPONENT_TYPE type, GameObject* game_object);
	~MaterialComponent();

	void ComponentEditor();

	uint GetTexture(uint n);
	void SetColor(float r, float g, float b, float a = 1.0f);


public:

	uint texture_id = 0;
	std::string path;
	std::vector<uint> textures;
	float color[4] = { 1.0f,1.0f,1.0f,1.0f };
};


