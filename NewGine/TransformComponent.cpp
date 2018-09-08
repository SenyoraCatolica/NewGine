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
}

void TransformComponent::SetGlobalTransform(math::float4x4 trans)
{
	global_transformation = trans;
}
//=============================================================================

void TransformComponent::Update(float dt)
{
	if (modify == true)
	{
		SetTransform();
		modify = false;
	}
}

void TransformComponent::ComponentEditor()
{
	//2DO
}






