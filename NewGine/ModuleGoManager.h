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

	bool Start();
	update_status PreUpdate();
	update_status Update();


	//GameObject Management
	GameObject* CreateEmpty(const char* name);
	GameObject* CreateGameObject(const char* name, GameObject* parent = nullptr);
	GameObject* CreateGameObject(const char* name, uint uuid, GameObject* parent, bool is_static, bool is_active);
	bool DeleteGameObject(GameObject* to_delete);
	GameObject* CreateCamera(const char* name, bool is_editor_cam);
	GameObject* GetCameraObjectInRoot(GameObject* root);
	bool HasCameraObjectInRoot(GameObject* root);
	void SwitchToGameCam();

	GameObject* Raycast(const Ray& ray)const;
	void SelectObject();
	void DrawLocator();

	GameObject* GetRoot();

	//Scene management
	void LoadScene(const char* name);
	void SaveScene(const char* name);
	void LoadEmptyScene();
	void ClearScene();
	void SaveSceneOnPlay();
	void LoadSceneOnStop();

	GameObject* LoadGameObject(const JSONWrapper& file);
	void LoadPrefab(const char* name);
	bool ClearGameObjectFromScene(GameObject* go);
	void LoadExtraComponent(GameObject* go, const char* file, COMPONENT_TYPE type);

	void TransformationHierarchy(GameObject* object);

public:
	std::list<GameObject*> all_gameobjects;
	std::list<GameObject*> objects_to_draw;
	std::list<GameObject*> dynamic_objects;
	std::list<GameObject*> todelete_objects;


	Quadtree* quadtree;

private:

	GameObject* root = nullptr;
	GameObject* selected_go = nullptr;
};

#endif // _GOMANAGER_H_
