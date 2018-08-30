#ifndef TRANSFORM_H
#define TRANSFORM_H


#include "Globals.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "Component.h"
#include "GameObject.h"

using namespace math;

class TransfomComponent : public Component
{
public:
	TransfomComponent(component_type type, GameObject* game_object);
	~TransfomComponent();


	void SetTranslation(float3 newpos);
	void SetTranslation(float x, float y, float z);
	float3 GetTranslation();

	void SetScale(float3 new_scale);
	void SetScale(float x, float y, float z);
	float3 GetScale();

	void SetRotation(float3 rot);
	void SetRotation(Quat rot);
	void SetRotation(float x, float y, float z, float w);
	float3 GetRotation();


	float4x4 GetTransformationMatrix();
	void SetTransform();

	math::float4x4 GetLocalTransform();
	math::float4x4 GetGlobalTranform();
	void SetGlobalTransform(math::float4x4 trans);


	void ComponentEditor();

	void Update(float dt);

public:
	 
	float3 position;
	float3 scale;
	Quat rotation;
	float3 rotation_degree;


	float4x4 local_transform = float4x4::identity;
	float4x4 global_transform = float4x4::identity;
	 
};

#endif // !TRANSFORM_H