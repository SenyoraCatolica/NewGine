
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
}

CameraComponent::~CameraComponent() {}

void CameraComponent::Update()
{
	//2DO
	// Handle visible object to draw

	UpdateLogicFrustum();
}

void CameraComponent::UpdateLogicFrustum()
{
	if (parent)
	{
		TransformComponent* trans = (TransformComponent*)parent->GetComponent(COMPONENT_TRANSFORM);
		float4x4 matrix = trans->GetGlobalTranform();
		frustum.SetPos(matrix.TranslatePart());
		frustum.SetFront(matrix.WorldZ());
		frustum.SetUp(matrix.WorldY());	
	}
	else
		LOG("Error: Component Camera is trying to update it's matrix but it is not attached to any game object.");

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

void CameraComponent::Save(JSONWrapper& file) const
{
	JSONWrapper array_value;
	array_value.WriteUInt("Type", type);
	array_value.WriteUInt("UUID", id);
	array_value.WriteBool("Enabled", enabled);
	array_value.WriteFloat("Near Plane", near_plane);
	array_value.WriteFloat("Far Plane", far_plane);
	array_value.WriteFloat("Horizontal FOV", horizontal_fov);
	array_value.WriteFloat("Veritcal FOV", vertical_fov);

	file.WriteArrayValue(array_value);
}

void CameraComponent::Load(JSONWrapper& file)
{
	type = (COMPONENT_TYPE)file.ReadUInt("Type");
	id = file.ReadUInt("UUID");
	enabled = file.ReadBool("Enabled");
	near_plane = file.ReadeFloat("Near Plane");
	far_plane = file.ReadeFloat("Far Plane");
	horizontal_fov = file.ReadeFloat("Horizontal FOV");
	vertical_fov = file.ReadeFloat("Vertical FOV");


	//Init frustum
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetPos({ 0, 0, 0 });
	frustum.SetFront({ 0, 0, 1 });
	frustum.SetUp({ 0, 1, 0 });
	frustum.SetViewPlaneDistances(5, 50);
	frustum.SetPerspective(DEGTORAD * horizontal_fov, DEGTORAD * horizontal_fov);
}




