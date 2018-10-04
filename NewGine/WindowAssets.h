#pragma once
#include "Window.h"
#include "Globals.h"
#include <vector>
#include "ModuleResourceManager.h"


struct AssetFile
{
	const char* directory_name = nullptr;
	const char* directory_name_next = nullptr;
	char* file_name = nullptr;
	FILE_TYPE type;
};

class WindowAssets : public Window
{
public:
	WindowAssets(Application* App);
	~WindowAssets();

	bool Start();

	void DrawOnEditor();

	const char* GetAssetsDirectory();
	void SetAssetsDirectory(const char* dir);

private:

	std::vector<AssetFile> assets;
	const char* assets_dir;
	int size = 50;

	//images
	uint mesh_icon;
	uint mat_icon;
};