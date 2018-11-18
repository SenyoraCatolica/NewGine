
#include "Imgui\imgui.h"
#include "OutlinerWindow.h"
#include "Application.h"

OutlinerWindow::OutlinerWindow(Application* app) : Window(app) {}

OutlinerWindow::~OutlinerWindow() {}

void OutlinerWindow::DrawOnEditor()
{
	if (active == false)
		return;

	ImGui::Begin("Outliner", 0);

	ImGui::SetWindowPos(ImVec2(0, 19));
	ImGui::SetWindowSize(ImVec2(App->window->screen_surface->w / 4, App->window->screen_surface->h / 3 * 2));


	if (App->go_manager->GetRoot() == NULL)
		ImGui::Text("No GameObjects on the Scene.");

	else
	{
		if (ImGui::TreeNodeEx("Root", ImGuiTreeNodeFlags_Framed))
		{
			if (App->go_manager->GetRoot() == App->editor->selected_object)
			{
				ImGuiTreeNodeFlags_Framed;
			}

			if (ImGui::IsItemClicked())
			{
				App->editor->selected_object = (App->go_manager->GetRoot());
			}

			//else
			ChildHierarchy(App->go_manager->GetRoot());

			ImGui::TreePop();
		}
	}

	ImGui::End();
}

void OutlinerWindow::ChildHierarchy(GameObject* parent)
{
	if (parent->childs.empty() == false)
	{
		std::vector<GameObject*>::const_iterator it = parent->childs.begin();
		while (it != parent->childs.end())
		{
			if ((*it) == App->editor->selected_object)
			{
				ImGuiTreeNodeFlags_Framed;
			}

			if ((*it)->childs.size() > 0)
			{
				if (ImGui::TreeNodeEx((*it)->name, ImGuiTreeNodeFlags_Framed))
				{
					if (ImGui::IsItemClicked())
					{
						App->editor->selected_object = (*it);
					}
					ChildHierarchy(*it);
					ImGui::TreePop();
				}
			}
			else
			{
				if (ImGui::TreeNodeEx((*it)->name, ImGuiTreeNodeFlags_Leaf))
				{
					if (ImGui::IsItemClicked())
					{
						App->editor->selected_object = (*it);
					}
					ImGui::TreePop();
				}
			}
			++it;
		}
	}
}