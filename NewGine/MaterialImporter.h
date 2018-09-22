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

	bool Import(const char* file, uint uuid = 0);
	MyTexture* LoadTexture(const char* file);


private:
	Application * App;
};
