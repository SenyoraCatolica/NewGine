#pragma once

#include "Application.h"
#include "ModuleImporter.h"
#include "ResourceMaterial.h"

struct MyTexture;

class MaterialImporter
{
public:

	MaterialImporter(Application* app);
	~MaterialImporter();

	bool Init();

	bool Import(const char* file, uint uuid = 0);
	MyTexture* LoadTexture(const char* file);
	bool LoadTexture(ResourceMaterial* mat);



private:
	Application * App;
};
