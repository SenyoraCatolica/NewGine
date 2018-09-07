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

	bool GetEnableTextures() const;
	bool GetEnableColorMaterial() const;
	bool GetEnableDepth() const;
	bool GetEnableCullFace() const;
	bool GetEnableLight() const;

	void SetEnableDepth(const bool active);
	void SetEnableCullFace(const bool active);
	void SetEnableLight(const bool active);
	void SetEnableTextures(const bool active);
	void SetEnableColorMaterial(const bool active);

	bool LoadConfig(JSON_Object* data);
	bool SaveConfig(JSON_Object* data)const;

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	bool draw_normals;
	bool draw_wireframe;
	bool draw_meshes;

private:

	bool enable_textures;
	bool enable_color_material;
	bool enable_depth;
	bool enable_face_culling;
	bool enable_lighting;

};