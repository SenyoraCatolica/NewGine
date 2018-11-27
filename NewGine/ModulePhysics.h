#pragma once

#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"
#include <list>

#define GRAVITY btVector3(0.0f, -10.0f, 0.0f) 

class PhysicBody3D;

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

	PhysicBody3D* AddBody(const Sphere_P& sphere, float mass = 1.0f);
	PhysicBody3D* AddBody(const Cube_P& cube, float mass = 1.0f);
	PhysicBody3D* AddBody(const Cylinder_P& cylinder, float mass = 1.0f);
	PhysicBody3D* AddBody(const Cube_P& cube, Module* listener, bool sensor, float mass);

public:


private:

	btDefaultCollisionConfiguration* collision_conf = nullptr;
	btCollisionDispatcher* dispatcher = nullptr;
	btBroadphaseInterface* broad_phase = nullptr;
	btSequentialImpulseConstraintSolver* solver = nullptr;
	btDiscreteDynamicsWorld* world = nullptr;
	DebugDrawer* debug_draw = nullptr;


	std::list<btCollisionShape*> shapes;
	std::list<PhysicBody3D*> bodies;
	std::list<btDefaultMotionState*> motions;
	std::list<btTypedConstraint*> constraints;

};


//=========================================================================================================
//small class to handle the debug drawing of the bullet physics

class DebugDrawer : public btIDebugDraw
{
public:
	DebugDrawer() : line(0, 0, 0)
	{}

	void DrawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void DrawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void ReportErrorWarning(const char* warningString);
	void Draw3dText(const btVector3& location, const char* textString);

	int	 GetDebugMode() const;
	void SetDebugMode(int debugMode);

	DebugDrawModes mode;
	Line_P line;
	Primitive point;
};