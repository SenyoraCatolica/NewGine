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
		ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.211f, 0.211f, 0.211f, 1.00f));
		if (ImGui::BeginChild(ImGui::GetID("Assets"), ImVec2(ImGui::GetWindowWidth(), 20)))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 2));
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20, 2));

			ImGui::SameLine(ImGui::GetWindowWidth() - 160);
			ImGui::PushItemWidth(100);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20, 2));
			ImGui::Text("Size:"); ImGui::SameLine();
			ImGui::SliderInt("##Size1", &size, 25, 100);
			ImGui::PopStyleVar();
			ImGui::PopItemWidth();
		}

	}
}

const char* WindowAssets::GetAssetsDirectory()
{

}
void WindowAssets::SetAssetsDirectory(const char* dir)
{

}