#include "Component.h"
#include "Globals.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "GameObject.h"
#include "GlobalFunctions.h"


Component::Component()
{}

Component::Component(COMPONENT_TYPE Type, GameObject* from) : type(Type), parent(from)
{
	id = GenerateUUID();
}

Component::Component(COMPONENT_TYPE Type, GameObject* from, uint number) : type(Type), parent(from), id(number)
{
	if (number <= 0)
	{
		id = GenerateUUID();
	}

	else
		id = number;
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

