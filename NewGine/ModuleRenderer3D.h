#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "Light.h"
#include "ModuleLoad.h"
#include "MaterialComponent.h"
#include "MeshComponent.h"
#include "TransformComponent.h"


#define MAX_LIGHTS 8

using namespace math;

class MyMesh;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height, float fovy);

	void DrawMesh(MeshComponent* mesh = NULL, TransfomComponent* transfrom = NULL, MaterialComponent* material = NULL);
	bool LoadMeshBuffer(const MyMesh* mesh);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

};