#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Globals.h"
#include "Component.h"
#include "Module.h"
#include <stdlib.h>
#include <list>
#include <vector>

class GameObject 
{
public:

	GameObject();
	GameObject(GameObject* Parent = NULL, std::string Name = "Empty GameObject");
	~GameObject();

	void Update(float dt);

	void AddComponent(Component* component);
	Component* CreateComponent(component_type type, uint id_num);
	bool DeleteComponent(Component* ComponentToDelete);
	Component* GetById(uint id);

	bool HasComponent(component_type type);
	Component* GetComponent(component_type type);

	

	GameObject* Duplicate(const GameObject GO_to_duplicate);
	void SetParent(GameObject* parent);

	void AddChild(GameObject* child);
	void RemoveChild(GameObject* child);



public:
	
	Component* transform = nullptr;
	Component* material = nullptr;
	Component* mesh = nullptr;

	std::vector<Component*> components;
	GameObject* parent;
	std::list<GameObject*> childs;
	
	std::string name;
};

#endif // GAMEOBJECT_H