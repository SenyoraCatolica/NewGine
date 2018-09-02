#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "Imgui\imgui.h"
#include "Glew\include\glew.h"
#include "glut\glut.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{};

ModuleEditor::~ModuleEditor()
{
};

bool ModuleEditor::Start()
{
	LOG("Loading Editor");
	bool ret = true;

	//Set Camera Position
	App->camera->Move(vec(10.0f, 6.0f, 30.0f));
	App->camera->LookAt(vec(-10, 10, 0));

	ShowTestWindow = false;
	depth, cull, light, color, texture, wireframe = false;

	return ret;
};

bool ModuleEditor::CleanUp()
{
	LOG("Unloading Editor");

	return true;
};

update_status ModuleEditor::Update(float dt)
{

	//Create World
	Plane_P grid(0, 1, 0, 0);
	grid.axis = true;
	grid.Render();

	//Create the menu bar
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Quit"))
			return UPDATE_STOP;


		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Help"))
	{

		if (ImGui::MenuItem("Documentation"))
			App->RequestBrowser("https://github.com/SenyoraCatolica/EngineMe/wiki");

		if (ImGui::MenuItem("Download"))
			App->RequestBrowser("https://github.com/SenyoraCatolica/EngineMe/releases");

		if (ImGui::MenuItem("Demo"))
			ShowTestWindow = !ShowTestWindow;

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("View"))
	{

		ImGui::Checkbox("Depth", &depth); ImGui::NewLine();
		ImGui::Checkbox("Cull", &cull); ImGui::NewLine();
		ImGui::Checkbox("Light", &light); ImGui::NewLine();
		ImGui::Checkbox("Color", &color); ImGui::NewLine();
		ImGui::Checkbox("Texture", &texture); ImGui::NewLine();
		ImGui::Checkbox("Wireframe", &wireframe); ImGui::NewLine();


		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("About"))
	{

		ImGui::Text("EngineMe");
		ImGui::Text("This is a small game engine developed by me as a student of videogames");
		ImGui::Text("Miquel Gironés Bosch");
		if (ImGui::MenuItem("Libraries"))
			AboutWindow = !AboutWindow;
		ImGui::Text("WTF is license?");

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

	if (ShowTestWindow)
		ImGui::ShowTestWindow();

	if (AboutWindow)
	{
		ImGui::Begin("Libraries");

		ImGui::Text("Bullet");
		ImGui::Text("Geolib");
		ImGui::Text("Imgui");
		ImGui::Text("SDL");
		ImGui::Text("Glew");
		ImGui::Text("Glut");



		ImGui::End();

	}

	return UPDATE_CONTINUE;
};
