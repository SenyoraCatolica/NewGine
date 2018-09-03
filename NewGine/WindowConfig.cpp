#include "WindowConfig.h"
#include "Application.h"
#include "Globals.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"


#include "Imgui/imgui.h"
#include "Glew\include\glew.h"
#include"MathGeoLib\include\MathGeoLib.h"


WindowConfig::WindowConfig(Application* app) : Window(app)
{
	//FPS Graphs
	App->SetMaxFPS(max_fps);
}

WindowConfig::~WindowConfig()
{
}


void WindowConfig::DrawOnEditor()
{
	int frames = App->GetFPS();

	if (fps.size() > 100) //Max seconds to show
	{
		for (int i = 1; i < fps.size(); i++)
		{
			fps[i - 1] = fps[i];
		}

		fps[fps.size() - 1] = frames;
	}
	else
	{
		fps.push_back(frames);
	}


	glGetIntegerv(GPU_CURRENT_AVAILABLE_MEM, &available_vram);
	available_vram /= 1000;
	usage_vram = initial_available_vram - available_vram;

	if (active)
	{

		ImGui::Begin("Configuration", &active);

		ImGui::SetWindowSize(ImVec2(400, 300));
		ImGui::SetWindowPos(ImVec2(1000, 550));

		ApplicationUI();
		WindowUI();
		Renderer();
		Camera();
		Input();

		ImGui::End();
	}
}


void WindowConfig::ApplicationUI()
{
	if (ImGui::CollapsingHeader("Application"))
	{
		ImGui::Spacing();
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Personalization:");
		ImGui::Spacing();


		ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Performance:");
		ImGui::Spacing();

		//Performance
		if (ImGui::SliderInt("Max FPS", &max_fps, 0, 60, NULL))
			App->SetMaxFPS(max_fps);

		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", fps [fps.size() - 1]);
		ImGui::PlotHistogram("##framerate", &fps[0], fps.size(), 0, NULL, 0.0f, 100.0f, ImVec2(310, 100));


		sprintf_s(title, 25, "Milliseconds %.1f", ms[ms.size() - 1]);
		ImGui::PlotHistogram("##framerate", &ms[0], ms.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
	}
}

void WindowConfig::WindowUI()
{
	if (ImGui::CollapsingHeader("Window"))
	{
		bool bool_tmp;
		int  int_tmp;

		int_tmp = App->window->GetWindowMode();
		if (ImGui::RadioButton("Resizable", &int_tmp, RESIZABLE))
			App->window->SetWindowMode(int_tmp);
		ImGui::SameLine();
		if (ImGui::RadioButton("Fullscreen Desktop", &int_tmp, FULL_DESKTOP))
			App->window->SetWindowMode(int_tmp);
		ImGui::SameLine();
		if (ImGui::RadioButton("Fullscreen", &int_tmp, FULLSCREEN))
			App->window->SetWindowMode(int_tmp);

		if (App->window->GetWindowMode() == RESIZABLE)
		{
			int_tmp = App->window->GetWidth();
			if (ImGui::SliderInt("Width", &int_tmp, 0, 1920))
			{
			App->window->SetWidth(int_tmp);
			}

			int_tmp = App->window->GetHeight();
			if (ImGui::SliderInt("Height", &int_tmp, 0, 1080))
			{
				App->window->SetHeight(int_tmp);
			}
		}
	}
}

void WindowConfig::Renderer()
{
	if (ImGui::CollapsingHeader("Render"))
	{
		bool bool_tmp;

		if (ImGui::TreeNodeEx("Geometry"))
		{
			bool_tmp = App->renderer3D->draw_normals;
			if (ImGui::Checkbox("Draw Normals", &bool_tmp))
			{
				App->renderer3D->draw_normals = bool_tmp;
			}

			bool_tmp = App->renderer3D->draw_wireframe;
			if (ImGui::Checkbox("Draw wireframe", &bool_tmp))
			{
				App->renderer3D->draw_wireframe = bool_tmp;
			}

			bool_tmp = App->renderer3D->draw_meshes;
			if (ImGui::Checkbox("Draw mesh", &bool_tmp))
			{
				App->renderer3D->draw_meshes = bool_tmp;
			}

			bool_tmp = App->renderer3D->GetEnableDepth();
			if (ImGui::Checkbox("Enable Depth", &bool_tmp))
			{
				App->renderer3D->SetEnableDepth(bool_tmp);
			}

			bool_tmp = App->renderer3D->GetEnableCullFace();
			if (ImGui::Checkbox("Enable Face Culling", &bool_tmp))
			{
				App->renderer3D->SetEnableCullFace(bool_tmp);
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("Textures"))
		{
			bool_tmp = App->renderer3D->GetEnableTextures();
			if (ImGui::Checkbox("Draw textures", &bool_tmp))
			{
				App->renderer3D->SetEnableTextures(bool_tmp);
			}

			bool_tmp = App->renderer3D->GetEnableColorMaterial();
			if (ImGui::Checkbox("Enable Color Material", &bool_tmp))
			{
			App->renderer3D->SetEnableColorMaterial(bool_tmp);
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("Lights"))
		{
			bool_tmp = App->renderer3D->GetEnableLight();
			if (ImGui::Checkbox("Enable Lightning", &bool_tmp))
			{
				App->renderer3D->SetEnableLight(bool_tmp);
			}
			ImGui::TreePop();
		}
	}
}


void WindowConfig::Camera()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		float3 pos = App->camera->GetPosition();

		ImGui::Text("X: %.3f   ", pos.x);
		ImGui::SameLine();
		ImGui::Text("Y: %.3f   ", pos.y);
		ImGui::SameLine();
		ImGui::Text("Z: %.3f", pos.z);

		float3 tmp_float3 = App->camera->GetReference();
		float tmp_float;

		if (ImGui::DragFloat3("Reference", tmp_float3.ptr()))
		{
			App->camera->LookAt(tmp_float3);
		}

		tmp_float = App->camera->speed;
		ImGui::DragFloat("Camera Speed", &tmp_float, 1.0f, 0.0f, 100.0f);
		App->camera->speed = tmp_float;

	}
}

void WindowConfig::Input()
{
	if (ImGui::CollapsingHeader("Input"))
	{
		ImGui::Text("X: %i   ", App->input->GetMouseX());
		ImGui::SameLine();
		ImGui::Text("Y: %i   ", App->input->GetMouseY());
	}
}