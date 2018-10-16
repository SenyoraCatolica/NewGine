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
		ImGui::Separator();
		ImGui::Checkbox("Bounding Box", &bb_active);
	}
}

void MeshComponent::Update(float dt)
{
	if (mesh != nullptr)
	{
		if (mesh->GetState() == MyResource::R_STATE::TO_DELETE)
			mesh = nullptr;

		if (bb_active)
			RecalculateBox();
	}			
}

void MeshComponent::SetResourceMesh(ResourceMesh * resourse_mesh)
{
	if (resourse_mesh != nullptr)
	{		
		mesh = resourse_mesh;
	}
}

void MeshComponent::RecalculateBox()
{
	TransformComponent* t = (TransformComponent*)parent->GetComponent(COMPONENT_TRANSFORM);
	math::OBB obb = local_box.Transform(t->GetGlobalTranform());
	global_box = obb.MinimalEnclosingAABB();
}
math::AABB MeshComponent::GetGlobalBox()
{
	return global_box;
}

void MeshComponent::SetBBActive(bool active)
{
	bb_active = active;
}


