#include "MeshComponent.h"
#include "Imgui\imgui.h"
#include "ModuleLoad.h"

struct MyMesh;

MeshComponent::MeshComponent(GameObject* game_object)
{
	parent = game_object;
}

MeshComponent::MeshComponent(component_type type, GameObject* game_object)
{
	this->type = type;
	parent = game_object;
}

MeshComponent::~MeshComponent()
{
	delete mesh;
	mesh = nullptr;
}

void MeshComponent::ComponentEditor()
{
	if (mesh != NULL)
	{
		ImGui::Text("Num vertices: %i", mesh->num_vertices);
		ImGui::Text("Num indices: %i", mesh->num_indices);
		ImGui::Text("Num normlas: %i", mesh->normals);
		ImGui::Text("Num UVs: %i", mesh->num_UVs);
	}
}