#include "MeshComponent.h"
#include "Imgui\imgui.h"


MeshComponent::MeshComponent(COMPONENT_TYPE type, GameObject* game_object) : Component(type, game_object, 1)
{
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
		path = resourse_mesh->path;
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

void MeshComponent::Save(JSONWrapper& file) const
{
	JSONWrapper array_value;
	array_value.WriteUInt("UUID", id);
	array_value.WriteBool("Enabled", enabled);
	array_value.WriteString("Path", mesh->path.c_str());

	array_value.WriteArrayValue(file);
}

void MeshComponent::Load(JSONWrapper& file)
{
	id = file.ReadUInt("UUID");
	enabled = file.ReadBool("Enabled");
	path = file.ReadString("Path");

	//2DO link resource with new path
}

