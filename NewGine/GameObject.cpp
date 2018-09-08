#include "GameObject.h"
#include "Globals.h"
#include "Glew\include\glew.h"

#include "TransformComponent.h"
#include "MeshComponent.h"
#include "MaterialComponent.h"
#include "CameraComponent.h"


GameObject::GameObject() {}
GameObject::~GameObject() {}

void GameObject::InitBuffer()
{
	//Vertices Buffer
	glGenBuffers(1, (GLuint*) &(mesh.id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.num_vertices * 3, mesh.vertices, GL_STATIC_DRAW);


	//Indices Buffer
	glGenBuffers(1, (GLuint*) &(mesh.id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh.num_indices, mesh.indices, GL_STATIC_DRAW);

	//Texture  Coords Buffer
	if (mesh.texture_coords != nullptr) // If the mesh has a texture, save it in the buffer
	{
		glGenBuffers(1, (GLuint*) &(mesh.id_texture_coords));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_texture_coords);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float2) * mesh.num_vertices, mesh.texture_coords, GL_STATIC_DRAW);
	}

	//Normals Buffer
	if (mesh.normals != nullptr) // If the mesh has normals, save them in the buffer
	{
		glGenBuffers(1, (GLuint*) &(mesh.id_normals));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_normals);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * mesh.num_vertices * 3, mesh.normals, GL_STATIC_DRAW);
	}
}


MyMesh GameObject::GetMesh()
{
	return mesh;
}

void GameObject::SetMesh(MyMesh m)
{
	mesh = m;
}


//Components===================================================================

Component* GameObject::AddComponent(COMPONENT_TYPE type, uint id_num)
{
	if (type == COMPONENT_TRANSFORM)
	{
		TransformComponent* transform = new TransformComponent(type, this);
		components.push_back(transform);
		return transform;
	}

	if (type == COMPONENT_MATERIAL)
	{
		MaterialComponent* material = new MaterialComponent(type, this);
		components.push_back(material);
		return material;
	}

	if (type == COMPONENT_MESH)
	{
		MeshComponent* mesh = new MeshComponent(type, this);
		components.push_back(mesh);
		return mesh;
	}

	if (type == COMPONENT_CAMERA)
	{
		CameraComponent* camera = new CameraComponent(type, this);
		components.push_back(camera);
		return camera;
	}
}

bool GameObject::DeleteComponent(Component* ComponentToDelete)
{
	bool ret = false;

	//2DO

	return ret;
}

bool GameObject::HasComponent(COMPONENT_TYPE type)
{
	std::list<Component*>::iterator it = components.begin();
	while (it != components.end())
	{
		if ((*it)->type == type)
		{
			return true;
		}
		it++;
	}
	return false;
}

Component* GameObject::GetComponent(COMPONENT_TYPE type)
{
	std::list<Component*>::iterator it = components.begin();
	while (it != components.end())
	{
		if ((*it)->type == type)
		{
			return (*it);
		}

		it++;
	}
	return nullptr;
}