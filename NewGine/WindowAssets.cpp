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
	return true;
}


void WindowAssets::DrawOnEditor()
{
	if (active)
	{
		ImGui::Begin("Assets", &active);

		std::vector<AssetFile>::iterator file = assets.begin();
		for (file; file != assets.end(); file++)
		{
			if (file->type != FILE_TYPE::NONE)
			{
				switch (file->type)
				{
					case FILE_TYPE::MESH:
						ImGui::Image((ImTextureID)mesh_icon, ImVec2(15, 15));
						ImGui::SameLine();
						//2DO Handle selected file
						break;

					case FILE_TYPE::MATERIAL:
						ImGui::Image((ImTextureID)mat_icon, ImVec2(15, 15));
						ImGui::SameLine();
						//2DO Handle selected file
						break;
				}
			}
		}
		ImGui::End();
	}
}

const char* WindowAssets::GetAssetsDirectory()
{
	if (assets_dir == nullptr)
		assets_dir = ASSETS_FOLDER;
	return assets_dir;
}

void WindowAssets::SetAssetsDirectory(const char* dir)
{
	assets_dir = dir;
}