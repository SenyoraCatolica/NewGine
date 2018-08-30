#include "TransformComponent.h"
#include "GameObject.h"
#include "Imgui\imgui.h"

TransfomComponent::TransfomComponent(component_type type, GameObject* game_object) : Component(type, game_object, 0)
{
	/*position.Set(0, 0, 0);
	scale.Set(1, 1, 1);
	rotation_degree.Set(0, 0, 0);
	rotation.Set(0, 0, 0, 0);*/
}

TransfomComponent::~TransfomComponent()
{

}

void TransfomComponent::SetTranslation(float3 new_pos)
{
	position.x = new_pos.x;
	position.y = new_pos.y;
	position.z = new_pos.z;

	SetTransform();
}

void TransfomComponent::SetTranslation(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;

	SetTransform();
}

float3 TransfomComponent::GetTranslation()
{
	return position;
}

void TransfomComponent::SetScale(float3 new_scale)
{
	scale.Set(new_scale.x, new_scale.y, new_scale.z);

	SetTransform();
}

void TransfomComponent::SetScale(float x, float y, float z)
{
	scale.Set(x, y, z);

	SetTransform();
}

float3 TransfomComponent::GetScale()
{
	return scale;
}

void TransfomComponent::SetRotation(float3 rot)
{
	rotation_degree = rot;

	float3 rotation_rad;

	rotation_rad.x = DegToRad(rotation_degree.x);
	rotation_rad.y = DegToRad(rotation_degree.y);
	rotation_rad.z = DegToRad(rotation_degree.z);

	rotation = Quat::FromEulerXYZ(rotation_rad.x, rotation_rad.y, rotation_rad.z);

	SetTransform();
}

void TransfomComponent::SetRotation(float x, float y, float z, float w)
{
	rotation.Set(x, y, z, w);

	SetTransform();
}

void TransfomComponent::SetRotation(Quat rot)
{
	rotation.Set(rot.x, rot.y, rot.z, rot.w);

	SetTransform();
}

float3 TransfomComponent::GetRotation()
{
	float3 tmp = rotation.ToEulerXYZ();

	RadToDeg(tmp.x);
	RadToDeg(tmp.y);
	RadToDeg(tmp.z);

	return tmp;
}


void TransfomComponent::ComponentEditor()
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
		SetRotation(new_rotation);
}

void TransfomComponent::Update(float dt)
{
	SetTransform();
}


float4x4 TransfomComponent::GetTransformationMatrix()
{
	return global_transform.Transposed();
}

void TransfomComponent::SetTransform()
{
	local_transform = local_transform.FromTRS(position, rotation, scale);
}

math::float4x4 TransfomComponent::GetLocalTransform()
{
	return local_transform;
}

math::float4x4 TransfomComponent::GetGlobalTranform()
{
	return global_transform;
}

void TransfomComponent::SetGlobalTransform(math::float4x4 trans)
{
	global_transform = trans;
}