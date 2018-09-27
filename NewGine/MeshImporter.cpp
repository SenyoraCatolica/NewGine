
#include "MeshImporter.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"
#include "ResourceMaterial.h"
#include "Globals.h"

#include "Glew\include\glew.h"

MeshImporter::MeshImporter(Application* app) : App(app){}
MeshImporter::~MeshImporter(){}

bool MeshImporter::Import(const aiScene * scene, const aiMesh* mesh, GameObject* go, const char* name, uint uuid = 0)
{
	MyMesh* m;

	MeshComponent* cMesh = (MeshComponent*)go->AddComponent(COMPONENT_MESH, GenerateUUID());


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
				MaterialComponent* cMat = (MaterialComponent*)go->AddComponent(COMPONENT_MATERIAL, GenerateUUID());

				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

				for (uint i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++)
				{
					aiString path;
					material->GetTexture(aiTextureType_DIFFUSE, i, &path);

					// 2DO:Path should be only the name here with texture folder

					ResourceMaterial* rMat = (ResourceMaterial*)App->resource_manager->TryGetResourceByName(path.C_Str()); //2DO this path should be name
					if (rMat != nullptr)
					{
						if (rMat->GetState() == MyResource::R_STATE::UNLOADED)
						{
							std::string temp = std::to_string(rMat->GetUUID());
							//App->importer->impMaterial->LoadResource(temp.c_str(), rMat); //2DO load res mat
						}

						//2Do merge rMat and cMat
					}
				}
			}
		}
	}

	else
	{
		LOG("WARNING! Could not find a mesh to import");
		return false;
	}
}

MyMesh* MeshImporter::LoadMesh(const char* path)
{
	MyMesh* mesh;

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
