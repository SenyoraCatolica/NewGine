#include "WindowAtributeEditor.h"
#include "Application.h"
#include "Imgui\imgui.h"


WindowAtributeEditor::WindowAtributeEditor(Application* app) : Window(app) {}

WindowAtributeEditor::~WindowAtributeEditor() {}

void WindowAtributeEditor::DrawOnEditor()
{/*2DO
	if (active)
	{
		ImGuiTreeNodeFlags attribute_editor_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
		ImGui::Begin("Attribute Editor", &active);

		ImGui::SetWindowSize(ImVec2(290, 650));
		ImGui::SetWindowPos(ImVec2(0, 18));

		uint count = 1;

		std::list<GameObject*>::const_iterator it = App->go_manager->all_gameobjects.begin();
		while (it != App->go_manager->all_gameobjects.end())
		{

			ImGui::Text("");
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Mesh %i", count);

			if (ImGui::CollapsingHeader("Transformation"));
			{
				//position--------------------------------------
				float3 p = (*it)->GetMesh().pos;
				float P[3] = { p.x, p.y, p.z };

				ImGui::Text("    X       Y        Z");
				ImGui::Separator();
				ImGui::Text("Position");
				ImGui::InputFloat3("", P);


				//rotation----------------------------------------
				float3 r = (*it)->GetMesh().rot;
				float R[3] = { r.x, r.y, r.z };

				ImGui::Text("Rotation");
				ImGui::InputFloat3("", R);

				//Scale----------------------------------------
				float3 s = (*it)->GetMesh().scale;
				float S[3] = { s.x, s.y, s.z };

				ImGui::Text("Scale");
				ImGui::InputFloat3("", S);


			}

			// Material
			if (ImGui::CollapsingHeader("Material"));
			{
				ImGui::Text("id: %d", (*it)->GetMesh().textures[0]->id);
				ImGui::Text("Width  %i", (*it)->GetMesh().textures[0]->width);
				ImGui::Text("Height  %i", (*it)->GetMesh().textures[0]->height);


				ImGui::Image((ImTextureID)(*it)->GetMesh().textures[0]->id, ImVec2(250, 250));
			}

			//Mesh
			if (ImGui::CollapsingHeader("Mesh"));
			{
				ImGui::Text("num vertices %i", (*it)->GetMesh().num_vertices);
				ImGui::Text("num indices %i", (*it)->GetMesh().num_indices);
				ImGui::Text("num texture coords %i", (*it)->GetMesh().num_texture_coords);
				ImGui::Text("num normals %i", (*it)->GetMesh().num_normals);
			}

			count++;
			it++;
		}

		ImGui::End();
	}*/
}