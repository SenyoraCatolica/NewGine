#pragma once

#include "MyResource.h"


struct MyTexture
{
	uint id;
	std::string path;
	uint width, height;
};

class ResourceMaterial : public MyResource
{
public:
	ResourceMaterial(uint uid);
	virtual ~ResourceMaterial();

	void GenerateResource(MyTexture* tex);

	bool LoadToMemory();
	void DeleteToMemory();

public:
	MyTexture * texture;

};