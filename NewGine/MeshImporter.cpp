
#include "MeshImporter.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"
#include "ResourceMaterial.h"
#include "Globals.h"

MeshImporter::MeshImporter(Application* app) : App(app){}
MeshImporter::~MeshImporter(){}

bool MeshImporter::Import(const aiScene * scene, const aiMesh* mesh, GameObject* go, const char* name, uint uuid = 0)
{
	MyMesh* m;

	MeshComponent* cMesh = (MeshComponent*)go->AddComponent(COMPONENT_MESH, GenerateUUID());


	if (mesh != nullptr)
	{
		App->console->AddMessage("Importing Mesh %s", name);
		
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
					App->console->AddMessage("WARNING, geometry face with != 3 indices!");
					return false;
				}
				else
				{
					memcpy(&m->indices[j * 3], mesh->mFaces[j].mIndices, 3 * sizeof(uint));
				}
			}
			App->console->AddMessage("Indices imported");

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
				App->console->AddMessage("Texture Coord imported");
			}

			//NORMALS  ============================================================
			if (mesh->HasNormals())
			{
				m->normals = new float[m->num_vertices * 3];
				memcpy(m->normals, mesh->mNormals, sizeof(float)*m->num_vertices * 3);
				m->num_normals = mesh->mNormals->Length();

				App->console->AddMessage("Normals Imported");
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
		App->console->AddMessage("WARNING! Could not find a mesh to import");
		return false;
	}
}