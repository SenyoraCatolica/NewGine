#pragma once

#include "Module.h"
#include "Globals.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

#define GRAVITY btVector3(0.0f, -10.0f, 0.0f) 

class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
};
