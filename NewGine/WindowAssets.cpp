#include "WindowAssets.h"
#include "Imgui/imgui.h"
#include "Application.h"



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
		ImGui::SetWindowSize(ImVec2(App->window->screen_surface->w / 4 * 3, App->window->screen_surface->h / 3));
		ImGui::SetWindowPos(ImVec2((0), App->window->screen_surface->h/3 * 2 + 19));

		ImGui::TextColored({ Green.r, Green.g,Green.b,Green.a }, "Scenes");

		scenes = App->file_system->GetFileNamesFromDirectory(ASSETS_FOLDER);
		for(std::vector<string>::iterator it = scenes.begin(); it != scenes.end(); it++)
		{
			size_t size = (*it).find_last_of(".");
			string scene_extension = (*it).substr(size + 1);

			if (scene_extension == "scn")
				ImGui::MenuItem((*it).data());
		}

		ImGui::Separator();

		ImGui::TextColored({ Green.r, Green.g,Green.b,Green.a }, "Prefabs");

		prefabs = App->file_system->GetFileNamesFromDirectory(ASSETS_FOLDER);
		for (std::vector<string>::iterator it = prefabs.begin(); it != prefabs.end(); it++)
		{
			size_t size = (*it).find_last_of(".");
			string prefab_extension = (*it).substr(size + 1);

			if (prefab_extension == "fbx")
				ImGui::MenuItem((*it).data());
		}

		ImGui::Separator();

		ImGui::TextColored({ Green.r, Green. g,Green. b,Green.a }, "Materials");

		materials = App->file_system->GetFileNamesFromDirectory(ASSETS_FOLDER);
		for (std::vector<string>::iterator it = materials.begin(); it != materials.end(); it++)
		{
			size_t size = (*it).find_last_of(".");
			string material_extension = (*it).substr(size + 1);
			if (material_extension == "png")
				ImGui::MenuItem((*it).data());
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