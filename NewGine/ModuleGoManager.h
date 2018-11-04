#pragma once
#ifndef _GOMANAGER_H_
#define _GOMANAGER_H_

#include "Globals.h"
#include "Module.h"
#include "GameObject.h"
#include "Quadtree.h"
#include <list>

class CameraComponent;

class ModuleGOManager : public Module
{
public:

	ModuleGOManager(Application* app, bool start_enabled = true);
	~ModuleGOManager();

	bool Init();
	update_status Update(float dt);


	//GameObject Management
	GameObject* CreateEmpty(const char* name);
	GameObject* CreateGameObject(const char* name, GameObject* parent = nullptr);
	bool DeleteGameObject(GameObject* to_delete);
	GameObject* CreateCamera(const char* name);
	GameObject* GetCameraObject();
	CameraComponent* GetCameraComponent();

	GameObject* Raycast(const Ray& ray)const;
	void SelectObject();
	void DrawLocator();


	GameObject* GetRoot();

public:
	std::list<GameObject*> all_gameobjects;
	std::list<GameObject*> objects_to_draw;
	std::list<GameObject*> dynamic_objects;

	Quadtree* quadtree;

private:
	std::list<GameObject*> to_delete_gos;

	GameObject* root = nullptr;
	GameObject* selected_go = nullptr;

	GameObject * camera = nullptr;
	CameraComponent* cam_comp = nullptr;
	GameObject* root = nullptr;
};

#endif // _GOMANAGER_H_
