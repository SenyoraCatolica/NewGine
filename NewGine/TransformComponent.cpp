#include "TransformComponent.h"
#include "GameObject.h"
#include "Imgui\imgui.h"
#include "Application.h"

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
	if (parent->parent != nullptr && parent->parent->HasComponent(COMPONENT_TRANSFORM) == true)
	{
		TransformComponent* parent_transform = (TransformComponent*)parent->parent->GetComponent(COMPONENT_TRANSFORM);
		global_transformation = GetTransformationMatrix() * parent_transform->GetGlobalTranform();
	}
	else
		global_transformation = GetTransformationMatrix();
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

	SetTranslation(file.ReadeFloat("PositionX"), file.ReadeFloat("PositionY"), file.ReadeFloat("PositionZ"));
	SetRotation(file.ReadeFloat("RotationX"), file.ReadeFloat("RotationY"), file.ReadeFloat("RotationZ"), file.ReadeFloat("RotationW"));
	SetScale(file.ReadeFloat("ScaleX"), file.ReadeFloat("ScaleY"), file.ReadeFloat("ScaleZ"));

	UpdateGlobalTransform();
}






