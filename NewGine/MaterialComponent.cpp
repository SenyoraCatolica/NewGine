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
	delete material;
	material = nullptr;
}

void MaterialComponent::ComponentEditor()
{
	ImGui::Text("Id Material: ");
	ImGui::SameLine();
	ImGui::Text("%d", material->texture->id);
	ImGui::Text("Width: ");
	ImGui::SameLine();
	ImGui::Text("%d", material->texture->width);
	ImGui::Text("Height: ");
	ImGui::SameLine();
	ImGui::Text("%d", material->texture->height);
	ImGui::Image((ImTextureID)material->texture->id, ImVec2(250, 250));
}


void MaterialComponent::SetColor(float r, float g, float b, float a)
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;
}

void MaterialComponent::SetResourceMaterial(ResourceMaterial* mat)
{
	material = mat;
	path = mat->path;
}

void MaterialComponent::Save(JSONWrapper& file) const
{
	JSONWrapper array_value;
	array_value.WriteUInt("Type", type);
	array_value.WriteUInt("UUID", id);
	array_value.WriteBool("Enabled", enabled);
	array_value.WriteString("Path", material->path.c_str());

	file.WriteArrayValue(array_value);
}

void MaterialComponent::Load(JSONWrapper& file)
{
	type = (COMPONENT_TYPE)file.ReadUInt("Type");
	id = file.ReadUInt("UUID");
	enabled = file.ReadBool("Enabled");
	path = file.ReadString("Path");

	//2DO link to resource mat using path
}