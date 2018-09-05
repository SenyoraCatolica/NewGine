#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "Imgui\imgui.h"
#include "Glew\include\glew.h"
#include "glut\glut.h"
#include "JSON\parson.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "editor";
};

ModuleEditor::~ModuleEditor()
{
};

bool ModuleEditor::Start()
{
	LOG("Loading Editor");
	bool ret = true;

	//Set Camera Position
	//App->camera->Move(vec(18.0f, 23.0f, -27.0f));
	//App->camera->LookAt(vec(-2, -1.5, 1.3));

	return ret;
};

bool ModuleEditor::CleanUp()
{
	LOG("Unloading Editor");

	return true;
};

update_status ModuleEditor::Update(float dt)
{
	HandleMainMenu();


	vector<Window*>::iterator it = windows.begin();
	while (it != windows.end())
	{
		(*it)->DrawOnEditor();
		++it;
	}

	ImGui::Render();
	return UPDATE_CONTINUE;
};

bool ModuleEditor::HandleMainMenu()
{
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File"))
	{

		ImGui::Separator();

		if (ImGui::MenuItem("Quit", "alt+f4"))
		{
			return UPDATE_STOP;
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Window"))
	{

		if (ImGui::Checkbox("Console", &console_active))
		{
			//Call Console  TODO
		}

		ImGui::Separator();

		if (ImGui::Checkbox("Configuration", &config_active))
		{
			configwindow->SetActive(config_active);
		}

		ImGui::Separator();

		if (ImGui::Checkbox("Atribute Editor", &atributeeditor_active))
		{
			atributeeditorwindow->SetActive(atributeeditor_active);
		}

		if (ImGui::Checkbox("Demo", &demo_active)){}

		ImGui::EndMenu();
	}

	if (demo_active)
		ImGui::ShowTestWindow();

	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("Documentation"))
		{
			ShellExecute(NULL, "open", "https://github.com/SenyoraCatolica/3DE/wiki", NULL, NULL, SW_MINIMIZE);
			LOG("Open Link");
		}

		ImGui::Separator();


		if (ImGui::MenuItem("Download Latest"))
		{
			ShellExecute(NULL, "open", "https://github.com/SenyoraCatolica/3DE/releases", NULL, NULL, SW_MINIMIZE);
			LOG("Open Link");
		}

		ImGui::Separator();


		if (ImGui::MenuItem("Report a bug"))
		{
			ShellExecute(NULL, "open", "https://github.com/SenyoraCatolica/3DE/issues", NULL, NULL, SW_MINIMIZE);
			LOG("Open Link");
		}

		ImGui::Separator();

		if (ImGui::MenuItem("About us"))
		{
			ImGui::Text("2Try-Engine");
			ImGui::Text("This is a small game engine developed by two student of videogames as a class project");
			ImGui::Text("Miquel Girones Bosch");
			ImGui::Text("Roger Homedes Rius");
			ImGui::Separator();
			if (ImGui::SmallButton("Instructions"))
				App->RequestBrowser("https://github.com/SenyoraCatolica/2Try-Engine/blob/master/README.md");

			ImGui::EndMenu();
		}

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

	return true;
}

void ModuleEditor::InitWindows()
{
	windows.push_back(configwindow = new WindowConfig(App));
	windows.push_back(atributeeditorwindow = new WindowAtributeEditor(App));
	//windows.push_back(console);  TODO
	
	//console_active = console->active;
}

bool ModuleEditor::LoadConfig(JSON_Object* data)
{
	bool ret = true;

	InitWindows();

	configwindow->active = json_object_get_boolean(data, "configuration_active");
	atributeeditorwindow->active = json_object_get_boolean(data, "atributeeditor_active");
	//TODO
	//demo
	//console

	config_active = configwindow->active;
	atributeeditor_active = atributeeditorwindow->active;

	return ret;
}

bool ModuleEditor::SaveConfig(JSON_Object* data) const
{
	bool ret = true;

	json_object_set_boolean(data, "configuration_active", configwindow->active);
	json_object_set_boolean(data, "atributeeditor_active", atributeeditorwindow->active);
	//TODO
	//demo
	//console

	return ret;
}
