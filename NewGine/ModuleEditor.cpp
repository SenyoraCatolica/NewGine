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

	SelectedObject = App->go_manager->GetRoot();


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

	/*console->SetActive(true);
	console->ConsoleEditor();*/
	AttributeEditor();
	Outliner();

	return UPDATE_CONTINUE;
};


void ModuleEditor::AttributeEditor()
{
	ImGuiTreeNodeFlags attribute_editor_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
	ImGui::Begin("Attribute Editor");

	ImGui::SetWindowSize(ImVec2(390, 600));
	ImGui::SetWindowPos(ImVec2(10, 25));

	if (SelectedObject == NULL)
		ImGui::Text("No GameObject Selected.");

	else
	{
		/*// Change the name
		char* new_name = new char[20];
		strcpy(new_name, SelectedObject->name);
		ImGui::InputText("Name", new_name, 20);
		strcpy(SelectedObject->name, new_name);*/


		//Transformation
		if (ImGui::CollapsingHeader("Transformation"));
		{
			if (SelectedObject->transform != NULL)
				SelectedObject->transform->ComponentEditor();

			else
				ImGui::Text("No transformation component.");
		}

		// Material
		if (ImGui::CollapsingHeader("Material"));
		{
			if (SelectedObject->material != NULL)
				SelectedObject->material->ComponentEditor();

			else
				ImGui::Text("No material component.");
		}

		//Mesh
		if (ImGui::CollapsingHeader("Mesh"));
		{
			if (SelectedObject->mesh != nullptr)
			{
				if (SelectedObject->mesh != NULL)
					SelectedObject->mesh->ComponentEditor();

				else
					ImGui::Text("No mesh component.");
			}
		}
	}

	ImGui::End();
}

void ModuleEditor::Outliner()
{

	ImGuiTreeNodeFlags outliner_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;
	ImGui::Begin("Outliner", 0, outliner_flags);

	ImGui::SetWindowSize(ImVec2(390, 600));
	ImGui::SetWindowPos(ImVec2(400, 25));


	if (App->go_manager->GetRoot() == NULL)
		ImGui::Text("No GameObjects on the Scene.");

	else
	{
		if (ImGui::TreeNodeEx("Root", ImGuiTreeNodeFlags_Framed))
		{
			if (App->go_manager->GetRoot() == SelectedObject)
			{
				ImGuiTreeNodeFlags_Framed;
			}

			if (ImGui::IsItemClicked())
			{
				SelectedObject = (App->go_manager->GetRoot());
			}

			//else
			ChildHierarchy(App->go_manager->GetRoot());

			ImGui::TreePop();
		}
	}

	ImGui::End();

}

void ModuleEditor::ChildHierarchy(GameObject* parent)
{
	if (parent->childs.empty() == false)
	{
		list<GameObject*>::const_iterator it = parent->childs.begin();
		while (it != parent->childs.end())
		{
			if ((*it) == SelectedObject)
			{
				ImGuiTreeNodeFlags_Framed;
			}

			if ((*it)->childs.size() > 0)
			{
				if (ImGui::TreeNodeEx((*it)->name.data(), ImGuiTreeNodeFlags_Framed))
				{
					if (ImGui::IsItemClicked())
					{
						SelectedObject = (*it);
					}
					ChildHierarchy(*it);
					ImGui::TreePop();
				}
			}
			else
			{
				if (ImGui::TreeNodeEx((*it)->name.data(), ImGuiTreeNodeFlags_Leaf))
				{
					if (ImGui::IsItemClicked())
					{
						SelectedObject = (*it);
					}
					ImGui::TreePop();
				}
			}
			++it;
		}
	}
}