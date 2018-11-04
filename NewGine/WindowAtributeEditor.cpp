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

	if (App->editor->selected_object == NULL)
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
	}

	ImGui::End();
}