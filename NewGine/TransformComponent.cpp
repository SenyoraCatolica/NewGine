#include "TransformComponent.h"
#include "GameObject.h"
#include "Imgui\imgui.h"
#include "Application.h"
#include "MeshComponent.h"

TransformComponent::TransformComponent(COMPONENT_TYPE type, GameObject* game_object) : Component(type, game_object, 0)
{
	position.Set(0, 0, 0);
	scale.Set(1, 1, 1);
	rotation_degree.Set(0, 0, 0);
	rotation.Set(0, 0, 0, 0);
}

TransformComponent::~TransformComponent()
{

}

//getters =========================================================

float3 TransformComponent::GetTranslation()
{
	return position;
}

float3 TransformComponent::GetRotation()
{
	float3 tmp = rotation.ToEulerXYZ();

	RadToDeg(tmp.x);
	RadToDeg(tmp.y);
	RadToDeg(tmp.z);

	return tmp;
}

Quat TransformComponent::GetQuatRotation()
{
	return rotation;
}

float3 TransformComponent::GetScale()
{
	return scale;
}

float4x4 TransformComponent::GetTransformationMatrix()
{
	return global_transformation.Transposed();
}

math::float4x4 TransformComponent::GetLocalTransform()
{
	return local_tranformation;
}

math::float4x4 TransformComponent::GetGlobalTranform()
{
	return global_transformation;
}

void TransformComponent::UpdateGlobalTransform()
{
	TransformComponent* trans = (TransformComponent*)parent->GetComponent(COMPONENT_TRANSFORM);

	if (trans != nullptr)
	{
		if (parent->parent != nullptr)
		{
			TransformComponent* parent_trans = (TransformComponent*)parent->parent->GetComponent(COMPONENT_TRANSFORM);
			trans->SetGlobalTransform(parent_trans->GetGlobalTranform() * trans->GetLocalTransform());
		}

		else
			trans->SetGlobalTransform(trans->GetLocalTransform());
	}

	if (parent->childs.size())
	{
		vector<GameObject*>::iterator it = parent->childs.begin();
		while (it != parent->childs.end())
		{
			TransformComponent* child_trans = (TransformComponent*)(*it)->GetComponent(COMPONENT_TRANSFORM);
			child_trans->UpdateGlobalTransform();
			it++;
		}
	}

	MeshComponent* mesh = (MeshComponent*)parent->GetComponent(COMPONENT_MESH);
	if (mesh != nullptr)
		mesh->RecalculateBox();

}

//setters==================================================================

void TransformComponent::SetTranslation(float3 new_pos)
{
	position.x = new_pos.x;
	position.y = new_pos.y;
	position.z = new_pos.z;

	modify = true;
}

void TransformComponent::SetTranslation(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;

	modify = true;

}

void TransformComponent::SetRotation(float3 rot)
{
	rotation_degree = rot;

	float3 rotation_rad;

	rotation_rad.x = DegToRad(rotation_degree.x);
	rotation_rad.y = DegToRad(rotation_degree.y);
	rotation_rad.z = DegToRad(rotation_degree.z);

	rotation = Quat::FromEulerXYZ(rotation_rad.x, rotation_rad.y, rotation_rad.z);

	modify = true;
}

void TransformComponent::SetRotation(float x, float y, float z, float w)
{
	rotation.Set(x, y, z, w);

	modify = true;

}

void TransformComponent::SetRotation(Quat rot)
{
	rotation.Set(rot.x, rot.y, rot.z, rot.w);

	modify = true;
}

void TransformComponent::SetScale(float3 new_scale)
{
	scale.Set(new_scale.x, new_scale.y, new_scale.z);

	modify = true;

}

void TransformComponent::SetScale(float x, float y, float z)
{
	scale.Set(x, y, z);

	modify = true;

}

void TransformComponent::SetTransform()
{
	local_tranformation = local_tranformation.FromTRS(position, rotation, scale);
	local_tranformation;
	UpdateGlobalTransform();
}

void TransformComponent::SetGlobalTransform(math::float4x4 trans)
{
	global_transformation = trans;
}
//=============================================================================

void TransformComponent::Update()
{
	if (modify == true)
	{
		SetTransform();
		modify = false;
	}
}

void TransformComponent::ComponentEditor()
{
	bool pactive = parent->IsStatic();
	ImGui::Checkbox("Static", &pactive);
	parent->SetStatic(pactive);

	float3 new_pos = position;
	ImGui::Text("Position");
	ImGui::Text("     X           Y            Z");
	if (ImGui::DragFloat3("pos", new_pos.ptr(), 0.5f))
		SetTranslation(new_pos);

	float3 new_scale = scale;
	ImGui::Text("Scale");
	ImGui::Text("     X           Y            Z");
	if (ImGui::DragFloat3("scl", new_scale.ptr(), 0.5f))
		SetScale(new_scale);

	float3 new_rotation = rotation_degree;
	ImGui::Text("Rotation");
	ImGui::Text("     X           Y            Z");
	if (ImGui::DragFloat3("rot", new_rotation.ptr(), 0.5f), 360, -360)
	{
		SetRotation(new_rotation);
	}

	//Local Matrix
	ImGui::Text("%0.2f %0.2f %0.2f %0.2f", local_tranformation.v[0][0], local_tranformation.v[0][1], local_tranformation.v[0][2], local_tranformation.v[0][3]);
	ImGui::Text("%0.2f %0.2f %0.2f %0.2f", local_tranformation.v[1][0], local_tranformation.v[1][1], local_tranformation.v[1][2], local_tranformation.v[1][3]);
	ImGui::Text("%0.2f %0.2f %0.2f %0.2f", local_tranformation.v[2][0], local_tranformation.v[2][1], local_tranformation.v[2][2], local_tranformation.v[2][3]);
	ImGui::Text("%0.2f %0.2f %0.2f %0.2f", local_tranformation.v[3][0], local_tranformation.v[3][1], local_tranformation.v[3][2], local_tranformation.v[3][3]);
}


void TransformComponent::Save(JSONWrapper& file) const
{
	JSONWrapper array_value;
	array_value.WriteUInt("Type", type);
	array_value.WriteUInt("UUID", id);
	array_value.WriteBool("Enabled", enabled);
	array_value.WriteMatrix("Loacal Matrix", local_tranformation);

	array_value.WriteFloat("PositionX", position.x);
	array_value.WriteFloat("PositionY", position.y);
	array_value.WriteFloat("PositionZ", position.z);

	array_value.WriteFloat("RotationX", rotation.x);
	array_value.WriteFloat("RotationY", rotation.y);
	array_value.WriteFloat("RotationZ", rotation.z);
	array_value.WriteFloat("RotationW", rotation.w);

	array_value.WriteFloat("ScaleX", scale.x);
	array_value.WriteFloat("ScaleY", scale.y);
	array_value.WriteFloat("ScaleZ", scale.z);


	file.WriteArrayValue(array_value);
}

void TransformComponent::Load(JSONWrapper& file)
{
	type = (COMPONENT_TYPE)file.ReadUInt("Type");
	id = file.ReadUInt("UUID");
	enabled = file.ReadBool("Enabled");
	local_tranformation = file.ReadMatrix("Local Matrix");

	float3 pos = float3::zero; 
	pos.x = file.ReadeFloat("PositionX"); 
	pos.y = file.ReadeFloat("PositionY"); 
	pos.z = file.ReadeFloat("PositionZ");
	SetTranslation(pos);

	Quat rot = Quat::identity;
	rot.x = file.ReadeFloat("RotationX");
	rot.y = file.ReadeFloat("RotationY");
	rot.z = file.ReadeFloat("RotationZ");
	rot.w = file.ReadeFloat("RotationW");
	SetRotation(rot);

	float3 sca = float3::zero;
	sca.x = file.ReadeFloat("ScaleX");
	sca.y = file.ReadeFloat("ScaleY");
	sca.z = file.ReadeFloat("ScaleZ");
	SetScale(sca);

	
	local_tranformation = local_tranformation.FromTRS(position, rotation, scale);
	UpdateGlobalTransform();
}






