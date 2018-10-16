
#include "CameraComponent.h"
#include "Application.h"
#include "Imgui\imgui.h"
#include "Glew\include\glew.h"


CameraComponent::CameraComponent(COMPONENT_TYPE type, GameObject* game_object) : Component(type, game_object, 3)
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	near_plane = 5;
	far_plane = 50;
	horizontal_fov = 60;
	vertical_fov = 60;
	frustum.SetPos({ 0, 0, 0 });
	frustum.SetFront({ 0, 0, 1 });
	frustum.SetUp({ 0, 1, 0 });
	frustum.SetViewPlaneDistances(5, 50);
	frustum.SetPerspective(DEGTORAD * horizontal_fov, DEGTORAD * horizontal_fov);

	id = GenerateUUID();
}

CameraComponent::~CameraComponent() {}

void CameraComponent::Update(float dt)
{
	//2DO
	// Handle visible object to draw
	//draw frustrum
	//update logic frustrum
}

void CameraComponent::ComponentEditor()
{
	float new_near_plane = near_plane;
	if (ImGui::DragFloat("Close Plane", &new_near_plane, 0.2f))
		SetNear(new_near_plane);

	float new_far_plane = far_plane;
	if (ImGui::DragFloat("Far Plane", &new_far_plane, 0.2f))
		SetFar(new_far_plane);

	float new_HFov = horizontal_fov;
	if (ImGui::DragFloat("Horizontal FOV", &new_HFov, 0.2f))
		SetHorizontalFOV(new_HFov);

	float new_VFov = vertical_fov;
	if (ImGui::DragFloat("Vertical FOV", &new_VFov, 0.2f))
		SetVerticalFOV(new_VFov);
}


const float CameraComponent::GetNear() const
{
	return frustum.NearPlaneDistance();
}

const float CameraComponent::GetFar() const
{
	return frustum.FarPlaneDistance();
}


void CameraComponent::SetNear(float value)
{
	frustum.SetViewPlaneDistances(value, frustum.FarPlaneDistance());
	near_plane = frustum.NearPlaneDistance();
}

void CameraComponent::SetFar(float value)
{
	frustum.SetViewPlaneDistances(frustum.NearPlaneDistance(), value);
	far_plane = frustum.FarPlaneDistance();
}

void CameraComponent::SetHorizontalFOV(float fov)
{
	frustum.SetVerticalFovAndAspectRatio(frustum.AspectRatio(), (fov * DEGTORAD));
	horizontal_fov = frustum.HorizontalFov();
}

void CameraComponent::SetVerticalFOV(float fov)
{
	frustum.SetVerticalFovAndAspectRatio((fov * DEGTORAD), frustum.AspectRatio());
	vertical_fov = frustum.VerticalFov();
}

void CameraComponent::LookAt(const math::float3& point)
{
	float3 vec = point - frustum.Pos();
	math::float3x3 matrix = math::float3x3::LookAt(frustum.Front(), vec, frustum.Up(), math::float3::unitY);
	frustum.SetFront(matrix.MulDir(frustum.Front()).Normalized());
	frustum.SetFront(matrix.MulDir(frustum.Up()).Normalized());
}

math::float4x4 CameraComponent::GetViewMatrix()const
{
	float4x4 tmp = frustum.ViewMatrix();
	tmp.Transpose();

	return tmp;
}

float4x4 CameraComponent::GetProjectionMatrix() const
{
	float4x4 tmp = frustum.ProjectionMatrix();
	tmp.Transpose();

	return tmp;
}


