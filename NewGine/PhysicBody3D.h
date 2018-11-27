#pragma once

#include <list>
#include "MathGeoLib/include/MathGeoLib.h"

class btRigidBody;
class Module;

class PhysicBody3D
{
public:
	PhysicBody3D(btRigidBody* body);
	~PhysicBody3D();

	void Push(float x, float y, float z);
	void Push(float3 dir);

	float4x4 GetTransform();
	float3 GetPos();

	void SetTransform(float4x4 matrix);
	float3 SetPos(float x, float y, float z);
	float3 SetPos(float3 pos);


private:
	float3 position = float3::zero;

public:
	std::list<Module*> collision_listeners;
	btRigidBody* body = nullptr;

};