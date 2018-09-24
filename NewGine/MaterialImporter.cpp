#include "Application.h"
#include "MaterialImporter.h"
#include "ResourceMaterial.h"


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

bool MaterialImporter::Import(const char* file, uint uuid = 0)
{
	bool ret = false;

	char* buffer;
	uint size = App->file_system->Load(file, &buffer);

	if (ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, size))
	{
		ILuint size_data;
		ILubyte *data;

		size_data = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer

		if (size_data > 0)
		{
			uint64_t nUUID;

			if (uuid = 0)
				nUUID = GenerateUUID();

			else
				nUUID = uuid;

			ResourceMaterial* rMat = (ResourceMaterial*)App->resource_manager->CreateResource(MyResource::R_TYPE::MATERIAL, nUUID);
		}

	}
}


MyTexture* LoadTexture(const char* file)
{
	//2DO GEt the correct name

	//setting image
	ILuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	//loading image
	ILboolean loaded = ilLoadImage(file);

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

		LOG("Load Texture on path %s with no errors", file);
	}

	//loading failed
	else
	{
		ILenum error = ilGetError();
		LOG("ERROR on path:%s ERROR: %s", file, iluErrorString(error))
	}


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());

	MyTexture* tex = new MyTexture();
	tex->id = texID;
	tex->width = ilGetInteger(IL_IMAGE_WIDTH);
	tex->height = ilGetInteger(IL_IMAGE_HEIGHT);
	//2DO
	//tex->name = name
	//tex->extension = extension

	ilDeleteImages(1, &texID);
	return tex;
}
