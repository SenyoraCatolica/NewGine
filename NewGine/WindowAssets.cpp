#include "WindowAssets.h"
#include "Imgui/imgui.h"


WindowAssets::WindowAssets(Application* app) : Window(app){}
WindowAssets::~WindowAssets()
{

}

bool WindowAssets::Start()
{
	//2DO Load icons image
	assets_dir = ASSETS_FOLDER;
}


void WindowAssets::DrawOnEditor()
{
	if (active)
	{
		ImGui::Begin("Assets", &active);
		ImGui::Text(assets_dir);

	}
}

const char* WindowAssets::GetAssetsDirectory()
{

}
void WindowAssets::SetAssetsDirectory(const char* dir)
{

}