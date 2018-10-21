#include "ResourceMesh.h"
#include "Glew\include\glew.h"


ResourceMesh::ResourceMesh(uint uid) : MyResource(uid, MyResource::R_TYPE::MESH, MyResource::R_STATE::UNLOADED)
{
	LOG("Resource Mesh Created!");
}

ResourceMesh::~ResourceMesh()
{
	delete mesh->vertices;
	delete mesh->indices;
	delete mesh->normals;
	delete mesh->texture_coords;
	delete mesh;
}

void ResourceMesh::GenerateResource(MyMesh* m)
{
	mesh = new MyMesh();

	//vertices
	mesh->id_vertices = m->id_vertices;
	mesh->num_vertices = m->num_vertices;
	mesh->vertices = m->vertices;

	//indices
	mesh->id_indices = m->id_indices;
	mesh->num_indices = m->num_indices;
	mesh->indices = m->indices;

	//normals
	mesh->id_normals = m->id_normals;
	mesh->num_normals = m->id_normals;
	mesh->normals = m->normals;

	//texture coords
	mesh->id_texture_coords = m->id_texture_coords;
	mesh->num_texture_coords = m->num_texture_coords;
	mesh->texture_coords = m->texture_coords;
}

bool ResourceMesh::LoadToMemory()
{
	//Vertices Buffer
	glGenBuffers(1, (GLuint*) &(mesh->id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertices * 3, mesh->vertices, GL_STATIC_DRAW);


	//Indices Buffer
	glGenBuffers(1, (GLuint*) &(mesh->id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->num_indices, mesh->indices, GL_STATIC_DRAW);

	//Texture  Coords Buffer
	if (mesh->texture_coords != nullptr) // If the mesh has a texture, save it in the buffer
	{
		glGenBuffers(1, (GLuint*) &(mesh->id_texture_coords));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_texture_coords);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float2) * mesh->num_vertices, mesh->texture_coords, GL_STATIC_DRAW);
	}

	//Normals Buffer
	if (mesh->normals != nullptr) // If the mesh has normals, save them in the buffer
	{
		glGenBuffers(1, (GLuint*) &(mesh->id_normals));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_normals);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * mesh->num_vertices * 3, mesh->normals, GL_STATIC_DRAW);
	}

	state = MyResource::R_STATE::LOADED;

	return true;
}

void ResourceMesh::DeleteToMemory()
{
	if (mesh->id_vertices != NULL)		glDeleteBuffers(1, &mesh->id_vertices);
	if (mesh->id_indices != NULL)			glDeleteBuffers(1, &mesh->id_indices);
	if (mesh->id_normals != NULL)	glDeleteBuffers(1, &mesh->id_normals);

	delete mesh->vertices;
	delete mesh->indices;
	delete mesh->normals;
	delete mesh->texture_coords;

	delete mesh;

	state = MyResource::R_STATE::UNLOADED;
}