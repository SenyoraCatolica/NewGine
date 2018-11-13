#pragma once

#include "MathGeoLib\include\MathGeoLib.h"
#include "Component.h"
#include "GameObject.h"

class TransformComponent : public Component
{
public:

	TransformComponent(COMPONENT_TYPE type, GameObject* game_object);
	~TransformComponent();

	//getters
	float3 GetTranslation();
	float3 GetRotation();
	Quat GetQuatRotation();
	float3 GetScale();

	float4x4 GetTransformationMatrix();
	float4x4 GetLocalTransform();
	float4x4 GetGlobalTranform();
	void UpdateGlobalTransform();


	//setters
	void SetTranslation(float3 newpos);
	void SetTranslation(float x, float y, float z);

	void SetRotation(float3 rot);
	void SetRotation(Quat rot);
	void SetRotation(float x, float y, float z, float w);

	void SetScale(float3 new_scale);
	void SetScale(float x, float y, float z);

	void SetTransform();
	void SetGlobalTransform(float4x4 trans);
	void ComponentEditor();

	void Update();

	void Save(JSONWrapper& file) const;
	void Load(JSONWrapper& file);

public:

	float3 position;
	float3 scale;
	Quat rotation;
	float3 rotation_degree;

	float4x4 local_tranformation = math::float4x4::identity;
	float4x4 global_transformation = math::float4x4::identity;


	bool modify = false;

};
