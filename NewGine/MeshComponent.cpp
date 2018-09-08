#include "MeshComponent.h"
#include "Imgui\imgui.h"

struct MyMesh;

MeshComponent::MeshComponent(GameObject* game_object)
{
	parent = game_object;
	local_box.SetNegativeInfinity();
	global_box.SetNegativeInfinity();
}

MeshComponent::MeshComponent(COMPONENT_TYPE type, GameObject* game_object)
{
	this->type = type;
	parent = game_object;
	local_box.SetNegativeInfinity();
	global_box.SetNegativeInfinity();
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
		ImGui::Text("Num UVs: %i", mesh->num_texture_coords);
	}
}

void MeshComponent::Update(float dt)
{
	if (mesh)
		RecalculateBox();
}

void MeshComponent::RecalculateBox()
{
	//2DO
}

math::AABB MeshComponent::GetGlobalBox()
{
	return global_box;
}

