#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "Light.h"



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

	void DrawMesh(MyMesh m);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	bool draw_normals = true;
	bool draw_wireframe = false;
	bool draw_meshes = true;

private:

	bool enable_textures = true;
	bool enable_color_material = true;
	bool enable_depth = true;
	bool enable_face_culling = false;
	bool enable_lighting = true;

};