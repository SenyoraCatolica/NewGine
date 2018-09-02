#include "GameObject.h"
#include "Globals.h"
#include "Glew\include\glew.h"


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