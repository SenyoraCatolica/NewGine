#pragma once
#ifndef _GOMANAGER_H_
#define _GOMANAGER_H_

#include "Globals.h"
#include "Module.h"
#include "GameObject.h"
#include <list>

class CameraComponent;

class ModuleGOManager : public Module
{
public:

	ModuleGOManager(Application* app, bool start_enabled = true);
	~ModuleGOManager();

	bool Init();
	update_status Update(float dt);

	GameObject* GetCameraObject();
	CameraComponent* GetCameraComponent();


public:
	std::list<GameObject*> all_gameobjects;
	std::list<GameObject*> objects_to_draw;

private:
	GameObject * camera = nullptr;
	CameraComponent* cam_comp = nullptr;
};

#endif // _GOMANAGER_H_
