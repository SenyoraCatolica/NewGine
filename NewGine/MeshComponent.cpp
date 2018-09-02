#include "MeshComponent.h"
#include "Imgui\imgui.h"

struct MyMesh;

MeshComponent::MeshComponent(GameObject* game_object)
{
	parent = game_object;
}

MeshComponent::MeshComponent(component_type type, GameObject* game_object)
{
	this->type = type;
	parent = game_object;
}

MeshComponent::~MeshComponent()
{
	delete mesh;
	mesh = nullptr;
}

void MeshComponent::ComponentEditor()
{
	if (mesh != NULL)
	{
		
	}
}