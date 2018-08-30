#pragma once
#ifndef _GOMANAGER_H_
#define _GOMANAGER_H_

#include "Globals.h"
#include "Module.h"
#include "GameObject.h"

class ModuleGOManager : public Module
{
public:

	ModuleGOManager(Application* app, bool start_enabled = true);
	~ModuleGOManager();

	bool Start();
	update_status Update(float dt);

	GameObject* CreateGameObject(GameObject* parent = NULL);
	GameObject* LoadGameObject(const char* path);
	void Delete(GameObject* GO_to_delete);
	void TransformationHierarchy(GameObject* object);
	GameObject*GetRoot();


public:
	std::list<GameObject*> all_gameobjects;


private:
	GameObject* root = NULL;


};

#endif // _GOMANAGER_H_
