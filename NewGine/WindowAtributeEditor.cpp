#include "WindowAtributeEditor.h"
#include "Application.h"
#include "Imgui\imgui.h"

#include "TransformComponent.h"
#include "MaterialComponent.h"
#include "MeshComponent.h"
#include "CameraComponent.h"


WindowAtributeEditor::WindowAtributeEditor(Application* app) : Window(app) {}

WindowAtributeEditor::~WindowAtributeEditor() {}

void WindowAtributeEditor::DrawOnEditor()
{
	if (active == false)
		return;

	//ImGuiTreeNodeFlags outliner_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;
	ImGui::Begin("Attribute Editor");

	ImGui::SetWindowSize(ImVec2(400, 500));
	ImGui::SetWindowPos(ImVec2(1000, 25));

	if (App->editor->selected_object == nullptr)
		ImGui::Text("No GameObject Selected.");

	else
	{
		//Transformation
		if (ImGui::CollapsingHeader("Transformation"));
		{
			TransformComponent* t = (TransformComponent*)App->editor->selected_object->GetComponent(COMPONENT_TRANSFORM);
			if (t != nullptr)
				t->ComponentEditor();

			else
				ImGui::Text("No transformation component.");
		}

		// Material
		if (ImGui::CollapsingHeader("Material"));
		{
			MaterialComponent* m = (MaterialComponent*)App->editor->selected_object->GetComponent(COMPONENT_MATERIAL);
			if (m != nullptr)
				m->ComponentEditor();

			else
				ImGui::Text("No material component.");
		}

		//Mesh
		if (ImGui::CollapsingHeader("Mesh"));
		{
			MeshComponent* m = (MeshComponent*)App->editor->selected_object->GetComponent(COMPONENT_MESH);

			if (m != nullptr)
				m->ComponentEditor();

			else
				ImGui::Text("No mesh component.");

		}

		//Camera
		if (ImGui::CollapsingHeader("Camera"));
		{
			CameraComponent* c = (CameraComponent*)App->editor->selected_object->GetComponent(COMPONENT_CAMERA);
			if (c != nullptr)
				c->ComponentEditor();

			else
				ImGui::Text("No camera component.");

		}

		ImGui::Separator();

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("Add Comp");

		if (ImGui::BeginPopup("Add Comp"))
		{
			AddComponentWindow();
			ImGui::EndPopup();
		}
	}

	ImGui::End();
}


void WindowAtributeEditor::AddComponentWindow()
{
	if (ImGui::BeginMenu("Component Mesh"))
	{
		mesh_names = App->file_system->GetFileNamesFromDirectory(MESH_FOLDER);

		std::vector<string>::iterator it = mesh_names.begin();
		while (it != mesh_names.end())
		{
			if (ImGui::MenuItem((*it).data()))
			{
				string complete_name = MESH_FOLDER + (*it);
				App->go_manager->LoadExtraComponent(App->editor->selected_object, complete_name.data(), COMPONENT_MESH);
			}

			it++;
		}

		mesh_names.clear();
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Component Material"))
	{
		material_names = App->file_system->GetFileNamesFromDirectory(MATERIAL_FOLDER);

		std::vector<string>::iterator it = material_names.begin();
		while (it != material_names.end())
		{
			if (ImGui::MenuItem((*it).data()))
			{
				string complete_name = MATERIAL_FOLDER + (*it);
				complete_name += "/" + (*it) + ".dds";
				App->go_manager->LoadExtraComponent(App->editor->selected_object, complete_name.data(), COMPONENT_MATERIAL);
			}

			it++;
		}

		material_names.clear();
		ImGui::EndMenu();
	}

	if (ImGui::MenuItem("Component Camera"))
	{
		App->editor->selected_object->AddComponent(COMPONENT_CAMERA);
	}
}
