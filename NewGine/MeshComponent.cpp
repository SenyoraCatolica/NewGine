#include "MeshComponent.h"
#include "Imgui\imgui.h"


MeshComponent::MeshComponent(COMPONENT_TYPE type, GameObject* game_object) : Component(type, game_object, 1)
{
	local_box.SetNegativeInfinity();
	global_box.SetNegativeInfinity();

}

MeshComponent::MeshComponent(MeshComponent& m)
{
	mesh = m.mesh;
	path = m.path;
	local_box =  m.local_box;
	global_box = m.global_box;
	bb_active = m.bb_active;
	parent = m.parent;
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
		ImGui::Separator();
		ImGui::Checkbox("Bounding Box", &bb_active);
		ImGui::Separator();

		if (ImGui::Button("delete##mesh"))
		{
			parent->DeleteComponent(this);
		}
	}
}

void MeshComponent::Update()
{
	if (mesh != nullptr)
	{
		//RecalculateBox();
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

void MeshComponent::RecalculateLocalbox()
{
	local_box.Enclose((float3*)mesh->mesh->vertices, mesh->mesh->num_vertices);
	//RecalculateBox();
}


void MeshComponent::RecalculateBox()
{
	TransformComponent* t = (TransformComponent*)parent->GetComponent(COMPONENT_TRANSFORM);
	math::OBB obb = local_box.Transform(t->GetGlobalTranform());
	global_box = obb.MinimalEnclosingAABB();
	parent->aabb = global_box;
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
	array_value.WriteUInt("Type", type);
	array_value.WriteUInt("UUID", id);
	array_value.WriteBool("Enabled", enabled);
	array_value.WriteString("Path", path.data());

	file.WriteArrayValue(array_value);
}

void MeshComponent::Load(JSONWrapper& file)
{
	type = (COMPONENT_TYPE)file.ReadUInt("Type");
	id = file.ReadUInt("UUID");
	enabled = file.ReadBool("Enabled");
	path = file.ReadString("Path");
}

