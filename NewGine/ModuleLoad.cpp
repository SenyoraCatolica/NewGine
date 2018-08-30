#include "Globals.h"
#include "Application.h"
#include "ModuleLoad.h"
#include "MaterialComponent.h"
#include "MeshComponent.h"
#include "TransformComponent.h"

#include "Glew\include\glew.h"
#include "glut\glut.h"


#include "Devil/include/il.h"
#include "Devil/include/ilut.h"
#pragma comment ( lib, "Devil/libx86/DevIL.lib" )
#pragma comment ( lib, "Devil/libx86/ILU.lib" )
#pragma comment ( lib, "Devil/libx86/ILUT.lib" )

#include "Assimp\include\cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

ModuleLoad::ModuleLoad(Application* app, bool start_enabled) : Module(app, start_enabled)
{};

ModuleLoad::~ModuleLoad()
{
};

bool ModuleLoad::Init()
{
	aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	//Initialize Devil
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	return true;
}

bool ModuleLoad::Start()
{
	LOG("Loading ModelLoader");
	bool ret = true;

	return ret;
};

bool ModuleLoad::CleanUp()
{
	LOG("Unloading ModelLoader");

	for (uint i = 0; i < Meshes.size(); i++)
		delete Meshes[i];

	Meshes.clear();

	aiDetachAllLogStreams();
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
};

update_status ModuleLoad::Update(float dt)
{
	return UPDATE_CONTINUE;
};


uint ModuleLoad::LoadTexture(const char* path)
{
	ILuint id;
	ilGenImages(1, &id);
	ilBindImage(id);
	ilLoadImage(path);

	return ilutGLBindTexImage();
	
}

GameObject* ModuleLoad::LoadNode(const aiNode* node, const aiScene* scene, GameObject* parent)
{			
	GameObject* game_object = App->go_manager->CreateGameObject(parent);
	game_object->name = node->mName.data;

	//transformation
	TransfomComponent* trans = (TransfomComponent*)game_object->CreateComponent(component_type::COMPONENT_TRANSFORM, 0);
	aiQuaternion rotation;
	aiVector3D scalar;
	aiVector3D position;

	node->mTransformation.Decompose(scalar, rotation, position);

	trans->SetTranslation(position.x, position.y, position.z);
	trans->SetScale(scalar.x, scalar.y, scalar.z);
	trans->SetRotation(rotation.x, rotation.y, rotation.z, rotation.w);
	



	if (scene->HasMeshes() == true)
	{
		for (uint i = 0; i < node->mNumMeshes; i++)
		{
			MeshComponent* comp_mesh = (MeshComponent*)game_object->CreateComponent(component_type::COMPONENT_MESH, 0);
			comp_mesh->mesh = LoadMesh(scene->mMeshes[node->mMeshes[i]], scene, game_object);

			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			//uint numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);
		
		}
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene, game_object);
	}

	return game_object;
}

MyMesh* ModuleLoad::LoadMesh(const aiMesh* mesh, const aiScene* scene, GameObject* new_object)
{

	//Mesh
	MyMesh* new_mesh = new MyMesh();

	//vertices
	glGenBuffers(1, (GLuint*)&(new_mesh->id_vertices));

	new_mesh->num_vertices = mesh->mNumVertices;
	new_mesh->vertices = new float[new_mesh->num_vertices * 3];
	memcpy(new_mesh->vertices, mesh->mVertices, sizeof(float) * new_mesh->num_vertices * 3);
	LOG("New mesh with %d vertices", new_mesh->num_vertices);

	glBindBuffer(GL_ARRAY_BUFFER, new_mesh->id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * new_mesh->num_vertices * 3, new_mesh->vertices, GL_STATIC_DRAW);



	//indices
	if (mesh->HasFaces())
	{
		glGenBuffers(1, (GLuint*)&(new_mesh->id_indices));


		new_mesh->num_indices = mesh->mNumFaces * 3;
		new_mesh->indices = new uint[new_mesh->num_indices]; // assume each face is a triangle
		for (uint i = 0; i < mesh->mNumFaces; ++i)
		{
			if (mesh->mFaces[i].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3 indices!");

			}

			else
				memcpy(&new_mesh->indices[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->id_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * new_mesh->num_indices, new_mesh->indices, GL_STATIC_DRAW);

	}


	//normals

	if (mesh->HasNormals())
	{
		glGenBuffers(1, (GLuint*)&(new_mesh->id_normals));

		new_mesh->num_normals = new_mesh->num_vertices;
		new_mesh->normals = new float[new_mesh->num_normals * 3];
		memcpy(new_mesh->normals, mesh->mNormals, sizeof(float) * new_mesh->num_normals * 3);

		glBindBuffer(GL_ARRAY_BUFFER, new_mesh->id_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * new_mesh->num_normals * 3, new_mesh->normals, GL_STATIC_DRAW);
	}


	if (mesh->HasTextureCoords(new_mesh->id_texture_coords))
	{
		new_mesh->num_texture_coords = mesh->mNumVertices;
		new_mesh->texture_coords = new float2[new_mesh->num_texture_coords];

		for (int i = 0; i < new_mesh->num_texture_coords; i++)
		{
			//Assign uv to the uvs_array<float2>
			new_mesh->texture_coords[i].x = mesh->mTextureCoords[new_mesh->id_texture_coords][i].x;
			new_mesh->texture_coords[i].y = mesh->mTextureCoords[new_mesh->id_texture_coords][i].y;
		}

		glGenBuffers(1, (GLuint*)&(new_mesh->id_texture_coords));
		glBindBuffer(GL_ARRAY_BUFFER, new_mesh->id_texture_coords);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float2) * new_mesh->num_texture_coords, new_mesh->texture_coords, GL_STATIC_DRAW);
	}


	if (scene->HasMaterials())
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

		if (path.length > 0)
		{

			MaterialComponent* comp_material = (MaterialComponent*)new_object->CreateComponent(component_type::COMPONENT_MATERIAL, 0);

			std::string complete_path = "Game";
			std::string tmp_path = path.data;
			tmp_path.erase(0, 2);
			complete_path = complete_path + tmp_path;

			comp_material->texture_id = App->load->LoadTexture(complete_path.data());
			comp_material->path = complete_path;
			LOG("Texture id %i Load: %s", comp_material->texture_id, path.data);
		}
	}

	Meshes.push_back(new_mesh);
	return new_mesh;
}