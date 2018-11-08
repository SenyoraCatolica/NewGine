
#include "MeshImporter.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"
#include "MyResource.h"
#include "ResourceMaterial.h"
#include "ResourceMesh.h"
#include "Globals.h"
#include "GameObject.h"
#include "GlobalFunctions.h"

#include "Glew/include/glew.h"

MeshImporter::MeshImporter(Application* app) : App(app){}
MeshImporter::~MeshImporter(){}

bool MeshImporter::Import(const char* file)
{
	bool ret = false;

	uint uuid = GenerateUUID();


	//Generate complete path in Library folder
	string mesh_path;
	mesh_path = MESH_FOLDER;
	mesh_path += std::to_string(uuid) + "/";
	App->file_system->CreateDir(mesh_path.c_str());
	//complete name of the file with path
	string lib_path = mesh_path + std::to_string(uuid) + ".dta";

	//Generate Asset folder for file
	string assets_path;
	if (App->file_system->Exists(file) == false)
		assets_path = App->resource_manager->CopyFileToAssets(file, App->editor->GetAssetsWindow()->GetAssetsDirectory());
	else
		assets_path = file;

	string path = file;

	//Generate Meta file 2DO
	App->resource_manager->CreateFileMeta(uuid, MESH, assets_path.data(), lib_path.data());
	
	//Importing starts here
	//=================================================================================
	char* buff;
	uint size = App->file_system->Load(assets_path.data(), &buff);

	if (size <= 0)
	{
		LOG("WARNING! Could not import file: %s", file);
		return ret;
	}

	const aiScene* scene = aiImportFile(file, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes() == true)
	{
		//Save the FBX as a new scene
		JSONWrapper new_scene_node;
		new_scene_node.WriteArray("Scene");

		GameObject* go = ImportNode(scene->mRootNode, scene, nullptr, mesh_path.data(), new_scene_node);

		char* buff;
		size_t size = new_scene_node.SerializeBuffer(&buff);

		App->file_system->Save(assets_path.data(), buff, size);

		delete[] buff;
	}
}

GameObject* MeshImporter::ImportNode(aiNode* node, const aiScene* scene, GameObject* parent, const char* save_path, JSONWrapper& root_node)
{
	GameObject* go = new GameObject();

	go->parent = parent;
	go->SetName(node->mName.C_Str());


	//Transform component==========================================================================
	TransformComponent* trans = (TransformComponent*)go->AddComponent(COMPONENT_TRANSFORM);
	DecomposeTransform(trans, node);
	//=============================================================================================

	//Save go info
	JSONWrapper go_node;

	go_node.WriteString("Name", go->name);
	go_node.WriteUInt("UUID", go->GetUID());
	go_node.WriteBool("Active", true);
	go_node.WriteBool("static", false);
	if (go->parent == nullptr)
		go_node.WriteUInt("Parent", 0);
	else
		go_node.WriteUInt("Parent", go->parent->GetUID());

	//Mesh component + Mesh import ================================================================


	for (uint i = 0; i < node->mNumMeshes; i++)
	{
		GameObject* new_go = nullptr;

		if (node->mNumMeshes > 1)
		{
			new_go = new GameObject();
			std::string name = "Mesh" + std::to_string(i);
			new_go->SetName(name.c_str());
			new_go->parent = parent;

			TransformComponent* t = (TransformComponent*)new_go->AddComponent(COMPONENT_TRANSFORM);
			DecomposeTransform(t, node);
		}

		else
			new_go = go;


		ImportMesh(scene, scene->mMeshes[node->mMeshes[i]], new_go, new_go->name, save_path, go_node);


		//Save components info in components array
		go_node.WriteArray("Components");

		//transform
		trans->Save(go_node);

		if (out_mesh != nullptr)
		{
			out_mesh->Save(go_node);
			out_mesh = nullptr;
		}

		if (out_material != nullptr)
		{
			out_material->Save(go_node);
			out_material = nullptr;
		}
	}

	root_node.WriteArrayValue(go_node);



	for (uint i = 0; i < node->mNumChildren; i++)
	{
		ImportNode(node->mChildren[i], scene, go, save_path, root_node);
	}

	return go;
}


bool MeshImporter::ImportMesh(const aiScene * scene, const aiMesh* mesh, GameObject* go, const char* name, const char* save_path, JSONWrapper& root, uint uuid)
{
	bool ret = false;

	MyMesh* m = new MyMesh();

	MeshComponent* mesh_comp = (MeshComponent*)go->AddComponent(COMPONENT_MESH);


	if (mesh != nullptr)
	{
		LOG("Importing Mesh %s", name);
		
		//VERTICES  ===============================================================
		m->num_vertices = mesh->mNumVertices;
		m->vertices = new float[m->num_vertices * 3];
		memcpy(m->vertices, mesh->mVertices, sizeof(float) * m->num_vertices * 3);

		if (mesh->HasFaces())
		{
			//INDICES  ===========================================================
			m->num_indices = mesh->mNumFaces * 3;
			m->indices = new uint[m->num_indices]; //Each face is a triangle

			for (uint j = 0; j < mesh->mNumFaces; j++)
			{
				if (mesh->mFaces[j].mNumIndices != 3)
				{
					LOG("WARNING, geometry face with != 3 indices!");
					return false;
				}
				else
				{
					memcpy(&m->indices[j * 3], mesh->mFaces[j].mIndices, 3 * sizeof(uint));
				}
			}
			LOG("Indices imported");

			//TEXTURE COORDS  =====================================================
			if (mesh->HasTextureCoords(0))
			{
				m->num_texture_coords = mesh->mNumVertices;
				m->texture_coords = new float2[m->num_texture_coords];


				for (int i = 0; i <m->num_texture_coords; i++)
				{
					//Assign uv to the uvs_array<float2>
					m->texture_coords[i].x = mesh->mTextureCoords[m->id_texture_coords][i].x;
					m->texture_coords[i].y = mesh->mTextureCoords[m->id_texture_coords][i].y;
				}
				LOG("Texture Coord imported");
			}

			//NORMALS  ============================================================
			if (mesh->HasNormals())
			{
				m->normals = new float[m->num_vertices * 3];
				memcpy(m->normals, mesh->mNormals, sizeof(float)*m->num_vertices * 3);
				m->num_normals = mesh->mNormals->Length();

				LOG("Normals Imported");
			}

			if (scene->HasMaterials())
			{
				MaterialComponent* mat = (MaterialComponent*)go->AddComponent(COMPONENT_MATERIAL);

				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

				for (uint i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++)
				{
					aiString path;
					material->GetTexture(aiTextureType_DIFFUSE, i, &path);

					// 2DO:Path should be only the name here with texture folder
					string mat_name = App->file_system->GetNameFromDirectory(path.C_Str());

					ResourceMaterial* resource_mat = (ResourceMaterial*)App->resource_manager->TryGetResourceByName(mat_name.c_str());
					if (resource_mat != nullptr)
					{
						if (resource_mat->GetState() == MyResource::R_STATE::UNLOADED)
						{
							// 2DO not clear this load
							App->importer->mat_importer->LoadTexture(resource_mat);
						}

						mat->material = resource_mat;
						out_material = mat;
					}
				}

			}
		}

		mesh_comp->Enable();
		mesh_comp->path = save_path;

		ResourceMesh* resource_mesh = (ResourceMesh*)App->resource_manager->CreateResource(MyResource::R_TYPE::MESH, GenerateUUID());
		if (ret = SaveMesh(m, name, save_path))
		{
			resource_mesh->mesh = m;
			resource_mesh->path = save_path;
		}
		mesh_comp->SetResourceMesh(resource_mesh);
		mesh_comp->path = mesh_comp->mesh->path;
		out_mesh = mesh_comp;
	}

	else
	{
		LOG("WARNING! Could not find a mesh to import");
		return ret;
	}

	return ret;
}

MyMesh* MeshImporter::LoadMesh(const char* path)
{
	MyMesh* mesh = new MyMesh();

	char* buffer;
	if (App->file_system->Load(path, &buffer) != 0)
	{
		mesh = new MyMesh();
		
		char* cursor = buffer;

		uint header[5];
		uint bytes = sizeof(header);
		memcpy(header, cursor, bytes);

		mesh->num_indices = header[0];
		mesh->num_vertices = header[1];
		mesh->num_texture_coords = header[3];

		//Indices
		cursor += bytes;
		bytes = sizeof(uint) * mesh->num_indices;
		mesh->indices = new uint[mesh->num_indices];
		memcpy(mesh->indices, cursor, bytes);

		//Vertices
		cursor += bytes;
		bytes = sizeof(float) * mesh->num_vertices * 3;
		mesh->vertices = new float[mesh->num_vertices * 3];
		memcpy(mesh->vertices, cursor, bytes);

		//Normals
		cursor += bytes;
		if (header[2] != 0)
		{
			bytes = sizeof(float) * mesh->num_vertices * 3;
			mesh->normals = new float[mesh->num_vertices * 3];
			memcpy(mesh->normals, cursor, bytes);

			cursor += bytes;
		}

		//Texture coords
		bytes = sizeof(float) * mesh->num_texture_coords * 2;
		mesh->texture_coords = new float2[mesh->num_texture_coords];
		memcpy(mesh->texture_coords, cursor, bytes);
		cursor += bytes;



		//Load mesh to VRAM=================================================

		//Vertices
		glGenBuffers(1, (GLuint*)&(mesh->id_vertices));
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->num_vertices, mesh->vertices, GL_STATIC_DRAW);

		//Indices 
		glGenBuffers(1, (GLuint*) &(mesh->id_indices));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->num_indices, mesh->indices, GL_STATIC_DRAW);

		//Normals 
		if (mesh->normals)
		{
			glGenBuffers(1, (GLuint*)&(mesh->id_normals));
			glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->num_vertices, mesh->normals, GL_STATIC_DRAW);
		}

		//Texture coords
		glGenBuffers(1, (GLuint*)&(mesh->id_texture_coords));
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_texture_coords);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * mesh->num_texture_coords, mesh->texture_coords, GL_STATIC_DRAW);

	}

	if (buffer)
		delete[] buffer;
	buffer = nullptr;

	return mesh;
}


bool MeshImporter::SaveMesh(MyMesh* m, const char* name, const char* save_path)
{
	bool ret = false;

	uint alloc[4] = { m->num_vertices, m->num_indices, m->num_texture_coords, m->num_normals  };
	uint size = sizeof(alloc) + (sizeof(float) * alloc[0] * 3) + (sizeof(uint) * alloc[1]) + sizeof(float2) * alloc[2];
	if (alloc[3] > 0)
		size += sizeof(float) * alloc[3];

	char* buffer = new char[size];
	char* cursor = buffer;

	//Header
	uint bytes = sizeof(alloc);
	memcpy(cursor, alloc, bytes);
	cursor += bytes;

	//Vertices
	bytes = sizeof(float) * alloc[0] * 3;
	memcpy(cursor, m->vertices, bytes);
	cursor += bytes;

	//Indices
	bytes = sizeof(uint) * alloc[1];
	memcpy(cursor, m->indices, bytes);
	cursor += bytes;

	//Texture Coords
	bytes = sizeof(float2) * alloc[3];
	memcpy(cursor, m->texture_coords, bytes);
	cursor += bytes;

	if (alloc[2] > 0)
	{
		memcpy(cursor, m->normals, bytes);
	}

	//2DO Create a better save function with uuid, folder and extension
	ret = App->file_system->Save(name, buffer, size, save_path, "mex");

	delete[] buffer;
	buffer = nullptr;

	return ret;
}


void MeshImporter::DecomposeTransform(TransformComponent* trans, aiNode* node)
{
	aiVector3D translation;
	aiQuaternion rotation;
	aiVector3D scaling;

	node->mTransformation.Decompose(scaling, rotation, scaling);


	math::float3 pos;
	pos.x = translation.x; pos.y = translation.y; pos.z = translation.z;

	math::Quat rot;
	rot.x = rotation.x; rot.y = rotation.y; rot.z = rotation.z; rot.w = rotation.w;

	math::float3 scale;
	scale.x = scaling.x; scale.y = scaling.y; scale.z = scaling.z;

	//Don't load fbx dummies as gameobjects. 
	static const char* dummies[5] =
	{
		"$AssimpFbx$_PreRotation",
		"$AssimpFbx$_Rotation",
		"$AssimpFbx$_PostRotation",
		"$AssimpFbx$_Scaling",
		"$AssimpFbx$_Translation"
	};

	for (int i = 0; i < 5; ++i)
	{
		if (((string)(node->mName.C_Str())).find(dummies[i]) != string::npos && node->mNumChildren == 1)
		{
			node = node->mChildren[0];
			node->mTransformation.Decompose(scaling, rotation, translation);
			pos += float3(translation.x, translation.y, translation.z);
			scale = float3(scale.x * scaling.x, scale.y * scaling.y, scale.z * scaling.z);
			rot = rot * Quat(rotation.x, rotation.y, rotation.z, rotation.w);
			i = -1;
		}
	}

	trans->SetTranslation(float3(pos.x, pos.y, pos.z));
	trans->SetRotation(Quat(rot.x, rot.y, rot.z, rot.w));
	trans->SetScale(float3(scale.x, scale.y, scale.z));
	trans->Enable();
}


