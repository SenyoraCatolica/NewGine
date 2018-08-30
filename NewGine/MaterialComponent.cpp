#include "MaterialComponent.h"
#include "imGUI\imgui.h"

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"


MaterialComponent::MaterialComponent(component_type type, GameObject* game_object)
{
	this->type = type;
	this->parent = game_object;
}

MaterialComponent::~MaterialComponent()
{

}

void MaterialComponent::ComponentEditor()
{

	ImGui::Text("Id Material: ");
	ImGui::SameLine();
	ImGui::Text("%d", texture_id);
	ImGui::Image((ImTextureID)texture_id, ImVec2(250, 250));


}

int MaterialComponent::LoadTexture(char* path)
{
	/*if (*path == '\0')  ei
	{
		return -1;
	}



	uint ID = ilutGLLoadImage(path);


	if (ID != 0)
	{
		int ret = textures.size();
		textures.push_back(ID);
		return ret;
	}
	else
	{
		LOG("Error loading texture %s", path);
	}*/
	return -1;

}

uint MaterialComponent::GetTexture(uint n)
{
	
		if (n >= 0 && n < textures.size())
		{
			return textures.at(n);
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

/*math::float4 MaterialComponent::GetColor()
{
	return math::float4(color);
}*/