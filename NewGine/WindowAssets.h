#pragma once
#include "Window.h"
#include "Globals.h"
#include <vector>


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
	const char* assets_dir = nullptr;

	std::vector<std::string> scenes;
	std::vector<std::string> prefabs;
	std::vector<std::string> materials;


};