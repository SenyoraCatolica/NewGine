#ifndef MESH
#define MESH

#include "Globals.h"
#include "Component.h"
#include "GameObject.h"

struct MyMesh;

class MeshComponent : public Component
{
public:
	MeshComponent(GameObject* game_object);
	MeshComponent(component_type type, GameObject* game_object);
	~MeshComponent();

	void ComponentEditor();

public:
	MyMesh* mesh = NULL;

	
};

#endif // MESH