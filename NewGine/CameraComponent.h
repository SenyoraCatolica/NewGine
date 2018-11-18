#pragma once

#pragma once

#include "Component.h"
#include "MathGeoLib\include\MathGeoLib.h"

class CameraComponent : public Component
{

public:

	CameraComponent(COMPONENT_TYPE type, GameObject* game_object);
	~CameraComponent();

	void Update();
	void UpdateLogicFrustum();

	const float GetNear()const;
	const float GetFar()const;

	void SetHorizontalFOV(float fov);
	void SetVerticalFOV(float fov);
	void SetNear(float value);
	void SetFar(float value);

	void LookAt(const math::float3& point);
	math::float4x4 GetViewMatrix()const;
	math::float4x4 GetProjectionMatrix()const;

	void ComponentEditor();

	void Save(JSONWrapper& file) const;
	void Load(JSONWrapper& file);

	math::Frustum frustum;

	bool culling = false;
	bool main_camera;

private:

	float near_plane;
	float far_plane;
	float horizontal_fov;
	float vertical_fov;

	math::float4x4 stop_transform = math::float4x4::identity;

};
