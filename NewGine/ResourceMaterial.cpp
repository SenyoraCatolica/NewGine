#include "ResourceMaterial.h"
#include "Glew\include\glew.h"

ResourceMaterial::ResourceMaterial(uint uid) : MyResource(uid, MyResource::R_TYPE::MATERIAL, MyResource::R_STATE::UNLOADED)
{
	LOG("Resource Material Created!");
}

ResourceMaterial::ResourceMaterial(uint uid, const char* path) : MyResource(uid, MyResource::R_TYPE::MATERIAL, MyResource::R_STATE::UNLOADED)
{
	this->path = path;
}


ResourceMaterial::~ResourceMaterial()
{
	delete texture;
}

void ResourceMaterial::GenerateResource(MyTexture* tex)
{
	texture = new MyTexture();
	texture->id = tex->id;
	texture->height = tex->height;
	texture->width = tex->width;
}

bool ResourceMaterial::LoadToMemory()
{
	state = MyResource::R_STATE::LOADED;
	return true;
}

void ResourceMaterial::DeleteToMemory()
{
	glDeleteTextures(1, &texture->id);
	delete texture;
	state = MyResource::R_STATE::UNLOADED;
}