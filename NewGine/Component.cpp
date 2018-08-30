#include "Component.h"
#include "Globals.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "MaterialComponent.h"

Component::Component()
{

}

Component::Component(component_type Type, GameObject* from, uint number) : type(Type), parent(from), id(number)
{
}

Component::Component(component_type Type, GameObject* from) : type(Type), parent(from)
{
	if(Type == COMPONENT_TRANSFORM)
		new TransfomComponent(Type, from);

	if (Type == COMPONENT_MESH)
		new MeshComponent(Type, from);

	if (Type == COMPONENT_MATERIAL)
		new MaterialComponent(Type, from);

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

