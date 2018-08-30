#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "SDL\include\SDL.h"

#include "Imgui\imgui.h"
#include "ModuleLoad.h"

#include "MathGeoLib\include\MathGeoLib.h"
#include "MathGeoLib\include\MathBuildConfig.h"

#include "Glew\include\glew.h"
#include "glut\glut.h"

#include "Random.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	//App->go_manager->LoadGameObject("Models/Town.fbx");

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	return UPDATE_CONTINUE;
}


