
#include "ModuleFBXLoader.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleGOManager.h"


//ASSIMP-----------------------
#include "Assimp\include\cimport.h"
#include "Assimp/include/postprocess.h"

#pragma comment(lib, "Assimp/libx86/assimp.lib")


//DEVIL
#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

#pragma comment(lib, "Devil/libx86/DevIl.lib")
#pragma comment(lib, "Devil/libx86/ILU.lib")
#pragma comment(lib, "Devil/libx86/ILUT.lib")


ModuleFBXLoader::ModuleFBXLoader(Application * app, bool start_enabled) : Module(app, start_enabled) {}

ModuleFBXLoader::~ModuleFBXLoader() {}

bool ModuleFBXLoader::Init()
{
	LOG("Creating FBX Loader context");

	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION ||
		iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION ||
		ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION)
	{
		LOG("DevIL version is different\n");
	}

	ilInit();
	iluInit();
	ilutInit();

	ilutRenderer(ILUT_OPENGL);

	return true;
}

void ModuleFBXLoader::LoadFBX(const char* filename, const char* texpath)
{
	aiMesh* new_mesh = nullptr;
	const aiScene* scene = aiImportFile(filename, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		//iterate all aiMesh structs
		for (uint i = 0; i < scene->mNumMeshes; i++)
		{
			go = new GameObject();
			App->go_manager->all_gameobjects.push_back(go);
			go->SetMesh(LoadMesh(scene->mMeshes[i], scene, texpath));
			go->InitBuffer();
		}
	}
}

MyMesh ModuleFBXLoader::LoadMesh(const aiMesh* mesh, const aiScene* scene, const char* texpath)
{
	MyMesh m;


	//VERTICES  ===============================================================
	m.num_vertices = mesh->mNumVertices;
	m.vertices = new float[m.num_vertices * 3];
	memcpy(m.vertices, mesh->mVertices, sizeof(float) * m.num_vertices * 3);


	if (mesh->HasFaces())
	{
		//INDICES  ===========================================================
		m.num_indices = mesh->mNumFaces * 3;
		m.indices = new uint[m.num_indices]; //Each face is a triangle

		for (uint j = 0; j < mesh->mNumFaces; j++)
		{
			if (mesh->mFaces[j].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3 indices!");
			}
			else
			{
				memcpy(&m.indices[j * 3], mesh->mFaces[j].mIndices, 3 * sizeof(uint));
			}
		}

		//TEXTURE COORDS  =====================================================
		if (mesh->HasTextureCoords(0))
		{
			m.num_texture_coords = mesh->mNumVertices;
			m.texture_coords = new float2[m.num_texture_coords];


			for (int i = 0; i <m.num_texture_coords; i++)
			{
				//Assign uv to the uvs_array<float2>
				m.texture_coords[i].x = mesh->mTextureCoords[m.id_texture_coords][i].x;
				m.texture_coords[i].y = mesh->mTextureCoords[m.id_texture_coords][i].y;
			}
		}

		//NORMALS  ============================================================
		if (mesh->HasNormals())
		{
			m.normals = new float[m.num_vertices * 3];
			memcpy(m.normals, mesh->mNormals, sizeof(float)*m.num_vertices * 3);
		}

		if (scene->HasMaterials())
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			aiString path;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &path);


			m.textures.clear();

			MyTexture* tex = new MyTexture();
			tex->path = path.C_Str();
			tex->id = LoadTexture(path.data);
			tex->width = ilGetInteger(IL_IMAGE_WIDTH);
			tex->height = ilGetInteger(IL_IMAGE_HEIGHT);

			m.textures.push_back(tex);
		}
	}

	return m;
}

uint ModuleFBXLoader::LoadTexture(const char* path)
{

	char complete_path[256] = TEXTURE_FOLDER;
	strcat(complete_path, path);

	//setting image
	ILuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	//loading image
	ILboolean loaded = ilLoadImage(complete_path);

	//success loading
	if (loaded == IL_TRUE)
	{
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		//convert image
		loaded = ilConvertImage(ilGetInteger(IL_IMAGE_FORMAT), IL_UNSIGNED_BYTE);

		LOG("Load Texture on path %s with no errors", complete_path);
	}

	//loading failed
	else
	{
		ILenum error = ilGetError();
		LOG("ERROR on path:%s ERROR: %s", complete_path, iluErrorString(error))
	}


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());


	ilDeleteImages(1, &texID);
	return texID;
}