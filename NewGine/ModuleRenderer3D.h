#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "Light.h"
#include "Quadtree.h"



#define MAX_LIGHTS 8

using namespace math;

struct MyMesh;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	void OnResize(int width, int height, float fovy);

	void DrawMesh(MyMesh m);
	void DrawGameObject(GameObject* go);

	void DebugDrawQuadtree(Quadtree* quadtree, QuadNode root);
	void DrawLocator(float4x4 transform, float4 color);
	void DrawFrustrum(const Frustum &frustum);

	void DrawBox(const AABB &aabb, Color color);
	void DrawBox(const vec* corners, float4 color);
	void DrawAABB(const vec* vertices, Color color);


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