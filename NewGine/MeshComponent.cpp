#include "MeshComponent.h"
#include "Imgui\imgui.h"

struct MyMesh;

MeshComponent::MeshComponent(COMPONENT_TYPE type, GameObject* game_object) : Component(type, game_object, 1)
{
	local_box.SetNegativeInfinity();
	global_box.SetNegativeInfinity();

	id = GenerateUUID();
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
		ImGui::Text("Num vertices: %i", mesh->mesh->num_vertices);
		ImGui::Text("Num indices: %i", mesh->mesh->num_indices);
		ImGui::Text("Num normlas: %i", mesh->mesh->normals);
		ImGui::Text("Num UVs: %i", mesh->mesh->num_texture_coords);
	}
}

void MeshComponent::Update(float dt)
{
	if (mesh->GetState() == MyResource::R_STATE::TO_DELETE)
		mesh = nullptr;		
}


void MeshComponent::SetResourceMesh(ResourceMesh * resourse_mesh)
{
	if (resourse_mesh != nullptr)
	{		
		mesh = resourse_mesh;
	}
}

