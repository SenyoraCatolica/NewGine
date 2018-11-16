#include "Application.h"
#include "MaterialImporter.h"
#include "ResourceMaterial.h"
#include "GlobalFunctions.h"
#include "MyResource.h"


//DEVIL
#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

#pragma comment(lib, "Devil/libx86/DevIl.lib")
#pragma comment(lib, "Devil/libx86/ILU.lib")
#pragma comment(lib, "Devil/libx86/ILUT.lib")


MaterialImporter::MaterialImporter(Application* app) : App(app) {}
MaterialImporter::~MaterialImporter()
{

}

bool MaterialImporter::Init()
{
	//Initialize Devil
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	return true;
}

bool MaterialImporter::Import(const char* file, uint uuid)
{
	bool rett = false;

	if(uuid == 0)
		uuid = App->GenerateUUID();


	//Generate complete path in Library folder
	string mat_path;
	string mat_name = App->file_system->DeleteExtensionFromName(App->file_system->GetNameFromDirectory(file));
	mat_path = MATERIAL_FOLDER;
	mat_path += mat_name;
	mat_path += "/";
	if(App->file_system->Exists(mat_path.c_str()) == false)
		App->file_system->CreateDir(mat_path.c_str());
	//complete name of the file with path
	string lib_path = mat_path + mat_name + ".dta";

	//Generate Asset folder for file
	string assets_path;
	if (App->file_system->Exists(file) == false)
		assets_path = App->resource_manager->CopyFileToAssets(file, App->editor->GetAssetsWindow()->GetAssetsDirectory());
	else
		assets_path = file;

	string path = file;

	//Generate Meta file 2DO
	App->resource_manager->CreateFileMeta(uuid, MATERIAL, assets_path.data(), lib_path.data());

	ResourceMaterial* mat = (ResourceMaterial*)App->resource_manager->CreateResource(MyResource::R_TYPE::MATERIAL, uuid);


	//Importing starts here
	//=================================================================================

	char* buffer;
	uint size = App->file_system->Load(assets_path.data(), &buffer);

	ILuint id;
	ilGenImages(1, &id);
	ilBindImage(id);

	if (ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, size))
	{
		ILuint size_data;
		ILubyte *data;

		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
		size_data = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer

		if (size_data > 0)
		{
			data = new ILubyte[size_data];
			if (ilSaveL(IL_DDS, data, size_data) > 0)
			{
				//string new_path = App->file_system->ChangeExtension(assets_path, ".dds");
				//rett = App->file_system->Save(new_path.data(), (char*)data, size_data);

				//if (rett)
				{
					string new_path = App->file_system->ChangeExtension(lib_path, ".dds");
					rett = App->file_system->Save(new_path.data(), (char*)data, size_data);
					mat->path = new_path;
					mat->name = App->file_system->GetNameFromDirectory(new_path.data());
				}

			}
			delete[] data;
			ilDeleteImages(1, &size);
		}
	}

	else
	{
		ILenum error = ilGetError();
		LOG("Image Load failed - IL reportes error: %i, %s", error, iluErrorString(error));
	}

	if(buffer)
		delete[] buffer;

	//Load texture to be get the resource material ready
	//mat->texture = LoadTexture(assets_path.data());

	return rett;
}


MyTexture* MaterialImporter::LoadTexture(const char* file)
{
	//setting image
	ILuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	//loading image
	ILboolean loaded = ilLoadImage(file);

	if (loaded == IL_TRUE)
	{
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
			iluFlipImage();

		loaded = ilConvertImage(ilGetInteger(IL_IMAGE_FORMAT), IL_UNSIGNED_BYTE);

		LOG("Load Texture on path %s with no errors", file);
	}

	else
	{
		ILenum error = ilGetError();
		LOG("ERROR on path:%s ERROR: %s", file, iluErrorString(error))
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), 
		ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());


	MyTexture* tex = new MyTexture();
	tex->id = texID;
	tex->width = ilGetInteger(IL_IMAGE_WIDTH);
	tex->height = ilGetInteger(IL_IMAGE_HEIGHT);

	ilDeleteImages(1, &texID);
	return tex;
}

bool MaterialImporter::LoadTexture(ResourceMaterial* mat)
{
	bool ret = false;
	ILenum error = ilGetError();


	char* buffer = nullptr;
	unsigned int size = App->file_system->Load(mat->texture->name.c_str(), &buffer);

	if (size > 0)
	{
		ILuint id;
		ilGenImages(1, &id);
		ilBindImage(id);

		if (ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, size))
		{
			ILinfo ImageInfo;
			iluGetImageInfo(&ImageInfo);
			if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
			{
				iluFlipImage();
			}

			// Convert the image into a suitable format to work with
			if (!ilConvertImage(ilGetInteger(IL_IMAGE_FORMAT), IL_UNSIGNED_BYTE))
			{
				error = ilGetError();
				LOG("Image conversion failed - IL reportes error: %i, %s", error, iluErrorString(error));
				exit(-1);
			}

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D,
				0,
				ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_WIDTH),
				ilGetInteger(IL_IMAGE_HEIGHT),
				0,
				ilGetInteger(IL_IMAGE_FORMAT),
				GL_UNSIGNED_BYTE,
				ilGetData());

			//Set texture properties
			mat->texture->id = id;
			mat->texture->width = ilGetInteger(IL_IMAGE_WIDTH);
			mat->texture->height = ilGetInteger(IL_IMAGE_HEIGHT);

			ilDeleteImages(1, &id);
			ret = true;
		}

		else
		{
			LOG("ERROR loading material: ERROR: %s", iluErrorString(error))
		}
	}
	else
	{
		LOG("Could load texture: %s", mat->texture->name.c_str());
	}

	delete[] buffer;

	return ret;
}

