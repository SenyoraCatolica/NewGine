#include "GameObject.h"
#include "Globals.h"
#include "Glew\include\glew.h"

#include "TransformComponent.h"
#include "MeshComponent.h"
#include "MaterialComponent.h"
#include "CameraComponent.h"


GameObject::GameObject() 
{
	uid = GenerateUUID();
	aabb.SetNegativeInfinity();
	originalAABB.SetNegativeInfinity();
	strcpy(name, "Unnamed");
}

GameObject::GameObject(const uint64_t uid)
{
	this->uid = uid;
	aabb.SetNegativeInfinity();
	originalAABB.SetNegativeInfinity();
	strcpy(name, "Unnamed");
}

GameObject::~GameObject() 
{
	//2DO implement destructor
}

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

const uint64_t GameObject::GetUID() { return uid; }

void GameObject::DrawLocator()
{
	//2DO implement locator
}

void GameObject::DrawAABB()
{
	if (aabb.IsFinite())
	{
		math::float3 corners[8];
		aabb.GetCornerPoints(corners);
	}
}

void GameObject::DrawOBB()
{
	if (obb.IsFinite())
	{
		math::float3 corners[8];
		obb.GetCornerPoints(corners);
	}
}

void GameObject::Select()
{
	//2DO implement select
}

void GameObject::Unselect()
{
	//2DO implement unselect
}

void GameObject::SetOriginalAABB()
{

	//2DO Enclose on a aabb the object if has mesh

	UpdateAABB();
}

void GameObject::UpdateAABB()
{
	aabb.SetNegativeInfinity();
	obb.SetNegativeInfinity();
	if (originalAABB.IsFinite())
	{
		obb = originalAABB;
		obb.Transform(transform->GetGlobalTranform().Transposed());
		aabb.Enclose(obb);
	}
}

void GameObject::UpdateTransformMatrix()
{
	//2DO

	if (HasComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM))
	{
		//Call Update global tranform
	}

	UpdateAABB();

	//Update cameras position
	

	//Call again for every child
}

void GameObject::SetActive(bool state)
{
	active = state;
}

bool GameObject::IsActive()
{
	return active;
}

void GameObject::SetStatic(bool Stat)
{
	is_static = Stat;
}

bool GameObject::IsStatic()
{
	return is_static;
}

void GameObject::SetName(const char* newName)
{
	strcpy(name, newName);
}

const char* GameObject::GetName()
{
	return name;
}


//Components===================================================================

Component* GameObject::AddComponent(COMPONENT_TYPE type)
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
