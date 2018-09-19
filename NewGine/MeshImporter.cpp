
#include "MeshImporter.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"
#include "Globals.h"

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

				}
			}

			aiQuaternion rotation;
			aiVector3D position;
			aiVector3D scale;

			scene->mRootNode->mTransformation.Decompose(scale, rotation, position);

			TransformComponent* cTrans = (TransformComponent*)go->AddComponent(COMPONENT_TRANSFORM, GenerateUUID());
			cTrans->SetTranslation(position.x, position.y, position.z);
			cTrans->SetRotation(rotation.x, rotation.y, rotation.z, rotation.w);
			cTrans->SetScale(scale.x, scale.y, scale.z);
		}
	}

	else
	{
		LOG("WARNING! Could not find a mesh to import");
		return false;
	}
}