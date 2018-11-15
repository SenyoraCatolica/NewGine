#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include <list>
#include "p2Point.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

enum Direction
{
	GO_RIGHT,
	GO_LEFT,
	GO_UP,
	GO_DOWN
};

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update();
	bool CleanUp();

	void Look(const vec &Position, const vec &Reference, bool RotateAroundReference = false);
	void LookAt(const vec &Spot);
	void Move(const vec &Movement);
	void Move(Direction d, float speed);
	float4x4 GetViewMatrix();

	void Rotate(float x, float y);

	vec GetPosition();
	vec GetReference();

	//Transform a 3D point to a point of the screen
	void From3Dto2D(vec point, int& x, int& y); 

	void UpdateEditorCam();

	void CleanCameras();

	void CreateEditorCam();

	CameraComponent* GetEditorCam();
	CameraComponent* GetCurrentCam();
	CameraComponent* GetGameCam();

	void SetEditorCam(CameraComponent* cam);
	void SetEditorCam(GameObject* cam);
	void SetCurrentCam(CameraComponent* cam);
	void SetGameCam(CameraComponent* cam);


	bool LoadConfig(JSON_Object* data);
	bool SaveConfig(JSON_Object* data) const;

private:

	void CalculateViewMatrix();

public:
	
	vec X, Y, Z, Position, Reference;
	float speed;

private:

	GameObject* editor_cam_go = nullptr;
	CameraComponent* current_cam = nullptr;
	CameraComponent* editor_cam = nullptr;
	CameraComponent* game_cam = nullptr;

	float4x4 ViewMatrix, ViewMatrixInverse;
};