#include "Component.h"
#include "Globals.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "GameObject.h"


Component::Component()
{
	if (id == 0)
		id = GenerateUUID();
}

Component::Component(COMPONENT_TYPE Type, GameObject* from) : type(Type), parent(from)
{
	//2DO

	/*if (Type == COMPONENT_TRANSFORM)
		new TransfomComponent(Type, from);

	if (Type == COMPONENT_MESH)
		new MeshComponent(Type, from);

	if (Type == COMPONENT_MATERIAL)
		new MaterialComponent(Type, from);
	*/
}

Component::Component(COMPONENT_TYPE Type, GameObject* from, uint number) : type(Type), parent(from), id(number)
{
	//2DO

	/*if (Type == COMPONENT_TRANSFORM)
	new TransfomComponent(Type, from);

	if (Type == COMPONENT_MESH)
	new MeshComponent(Type, from);

	if (Type == COMPONENT_MATERIAL)
	new MaterialComponent(Type, from);
	*/
}

Component::~Component()
{
}

void Component::Enable()
{
	enabled = true;
}

void Component::Disable()
{
	enabled = false;
}

GameObject* Component::GetObjectParent()
{
	return parent;
}

void Component::Save(JSONWrapper& file) const{}
void Component::Load(JSONWrapper& file){}

