#include "MaterialComponent.h"
#include "imGUI\imgui.h"

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"


MaterialComponent::MaterialComponent(COMPONENT_TYPE type, GameObject* game_object) : Component(type, game_object, 2)
{

}

MaterialComponent::~MaterialComponent()
{

}

void MaterialComponent::ComponentEditor()
{
	//2DO
}

uint MaterialComponent::GetTexture(uint id)
{

	if (id >= 0 && id < textures.size())
	{
		return textures.at(id);
	}
	else
	{
		return 0;
	}

}

void MaterialComponent::SetColor(float r, float g, float b, float a)
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;
}
