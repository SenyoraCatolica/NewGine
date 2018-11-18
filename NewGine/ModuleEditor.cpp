#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "Imgui\imgui.h"
#include "Imgui\ImGuizmo.h"
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

	return true;
};

bool ModuleEditor::CleanUp()
{
	LOG("Unloading Editor");

	return true;
};

update_status ModuleEditor::Update()
{
	HandleMainMenu();
	HandleScenesMenu();
	GamePanel();

	vector<Window*>::iterator it = windows.begin();
	while (it != windows.end())
	{
		(*it)->DrawOnEditor();
		++it;
	}

	HandleGuizmo();

	return UPDATE_CONTINUE;
};

bool ModuleEditor::HandleMainMenu()
{
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New Scene##NewMenuBar"))
		{
			want_new_scene = true;
		}
		if (ImGui::MenuItem("Save Scene##SaveMenuBar"))
		{
			want_to_save = true;
		}
		if (ImGui::MenuItem("Load Scene##LoadMenuBar"))
		{
			want_to_load = true;
		}

		if (ImGui::MenuItem("Load Prefab##LoadMenuBar"))
		{
			want_to_load_prefab = true;
		}

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
			//Call Console  2DO
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

		if (ImGui::Checkbox("Assets", &assets_active))
		{
			assetswindow->SetActive(assets_active);
		}

		ImGui::Separator();

		if (ImGui::Checkbox("Outliner", &outliner_active))
		{
			outlinerwindow->SetActive(outliner_active);
		}

		ImGui::Separator();

		if (ImGui::Checkbox("Demo", &demo_active)){}

		ImGui::EndMenu();
	}

	if (demo_active)
		ImGui::ShowTestWindow();

	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("Documentation"))
		{
			ShellExecute(NULL, "open", "https://github.com/SenyoraCatolica/NewGine/wiki", NULL, NULL, SW_MINIMIZE);
			LOG("Open Link");
		}

		ImGui::Separator();


		if (ImGui::MenuItem("Download Latest"))
		{
			ShellExecute(NULL, "open", "https://github.com/SenyoraCatolica/NewGine/releases", NULL, NULL, SW_MINIMIZE);
			LOG("Open Link");
		}

		ImGui::Separator();


		if (ImGui::MenuItem("Report a bug"))
		{
			ShellExecute(NULL, "open", "https://github.com/SenyoraCatolica/NewGine/issues", NULL, NULL, SW_MINIMIZE);
			LOG("Open Link");
		}

		ImGui::Separator();

		if (ImGui::MenuItem("About us"))
		{
			ImGui::Text("2Try-Engine");
			ImGui::Text("This is a small game engine developed by a student of videogames as a class project");
			ImGui::Text("Miquel Girones Bosch");
			ImGui::Separator();
			if (ImGui::SmallButton("Instructions"))
				App->RequestBrowser("https://github.com/SenyoraCatolica/NewGine/blob/master/README.md");

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
	windows.push_back(assetswindow = new WindowAssets(App));
	windows.push_back(outlinerwindow = new OutlinerWindow(App));
	//windows.push_back(console);  2DO
	
	//console_active = console->active;
}

void ModuleEditor::GamePanel()
{
	ImGui::SetNextWindowPos(ImVec2(500.0f, 20.0f));
	ImGui::SetNextWindowSize(ImVec2(300.0f, 30.0f));

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar;

	ImGui::Begin("PlayButtons", 0, ImVec2(500, 200), 0.8f, flags);

	if (App->GetGameState() != GAME_STATE::PLAY_STATE)
	{
		if (ImGui::Button("Play##PlayButton"))
		{
			selected_object = nullptr;
			App->Play();
		}
	}
	else
	{
		if (ImGui::Button("Pause##PauseButton"))
		{
			App->Pause();
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop##StopButton"))
		{
			selected_object = nullptr;
			App->Stop();
		}
	}
	ImGui::End();
}


bool ModuleEditor::LoadConfig(JSON_Object* data)
{
	bool ret = true;

	InitWindows();

	configwindow->active = json_object_get_boolean(data, "configuration_active");
	atributeeditorwindow->active = json_object_get_boolean(data, "atributeeditor_active");
	demo_active = json_object_get_boolean(data, "demo_active");
	assetswindow->active = json_object_get_boolean(data, "assets_active");
	outlinerwindow->active = json_object_get_boolean(data, "outliner_active");
	//2DO
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
	json_object_set_boolean(data, "demo_active", demo_active);
	json_object_set_boolean(data, "assets_active", assetswindow->active);
	json_object_set_boolean(data, "outliner_active", outlinerwindow->active);


	//2DO
	//console

	return ret;
}


WindowAssets* ModuleEditor::GetAssetsWindow()
{
	return assetswindow;
}

void ModuleEditor::HandleScenesMenu()
{
	ImGui::SetNextWindowSize(ImVec2(300, 120));
	if (ImGui::BeginPopupModal("New scene"))
	{
		bool close = false;
		ImGui::Text("Save current scene?");
		if (ImGui::Button("Yes##saveCurrentButton"))
		{
			want_to_save = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("No##NotSaveCurrentButton"))
		{
			selected_object = nullptr;
			App->go_manager->LoadEmptyScene();
			close = true;
		}
		ImGui::SameLine();
		if (close || ImGui::Button("Cancel##CancelSaveCurrentButton"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}


	ImGui::SetNextWindowSize(ImVec2(300, 120));
	if (ImGui::BeginPopupModal("Save scene"))
	{
		bool close = false;
		ImGui::Text("Scene name:");
		ImGui::InputText("##saveSceneInputText", scene_name, 256);
		if (ImGui::Button("Save##saveButton") && scene_name[0] != '\0')
		{
			App->go_manager->SaveScene(scene_name);
			selected_object = nullptr;
			close = true;
		}
		ImGui::SameLine();
		if (close || ImGui::Button("Cancel##cancelSaveScene"))
		{
			strcpy(scene_name, "");
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	ImGui::SetNextWindowSize(ImVec2(300, 120));
	if (ImGui::BeginPopupModal("Load Scene"))
	{
		ImGui::Text("Scene name:");
		ImGui::InputText("##saveSceneInputText", scene_name, 256);
		bool close = false;
		if (ImGui::Button("Load##loadButton") && scene_name[0] != '\0')
		{
			selected_object = nullptr;
			App->go_manager->LoadScene(scene_name);
			close = true;
		}
		ImGui::SameLine();
		if (close || ImGui::Button("Cancel##cancelLoadScene"))
		{
			strcpy(scene_name, "");
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	ImGui::SetNextWindowSize(ImVec2(300, 120));
	if (ImGui::BeginPopupModal("Load Prefab"))
	{
		ImGui::Text("Prefab name:");
		ImGui::InputText("##saveSceneInputText", prefab_name, 256);
		bool close = false;
		if (ImGui::Button("Load##loadButton") && prefab_name[0] != '\0')
		{
			selected_object = nullptr;
			App->go_manager->LoadPrefab(prefab_name);
			close = true;
		}
		ImGui::SameLine();
		if (close || ImGui::Button("Cancel##cancelLoadScene"))
		{
			strcpy(prefab_name, "");
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (want_new_scene)
	{
		ImGui::OpenPopup("New scene");
		want_new_scene = false;
	}
	if (want_to_save)
	{
		ImGui::OpenPopup("Save scene");
		want_to_save = false;
	}
	if (want_to_load)
	{
		ImGui::OpenPopup("Load Scene");
		want_to_load = false;
	}

	if (want_to_load_prefab)
	{
		ImGui::OpenPopup("Load Prefab");
		want_to_load_prefab = false;
	}
}

void ModuleEditor::HandleGuizmo()
{
	if (selected_object != nullptr)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::Enable(true);
		static ImGuizmo::OPERATION operation(ImGuizmo::TRANSLATE);
		ImGuizmo::SetRect(300, 25, 550, 1000);
		ImGuizmo::MODE transform_mode = ImGuizmo::LOCAL;


		if (io.WantTextInput == false)
		{
			// SET GUIZMO OPERATION ----------------------------------
			if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
			{
				operation = ImGuizmo::TRANSLATE;
			}
			else if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
			{
				operation = ImGuizmo::ROTATE;
			}
			else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			{
				operation = ImGuizmo::SCALE;
			}
		}

		TransformComponent* t = (TransformComponent*)selected_object->GetComponent(COMPONENT_TRANSFORM);
		TransformComponent* parent_t = (TransformComponent*)selected_object->parent->GetComponent(COMPONENT_TRANSFORM);
		CameraComponent* camera = App->camera->GetCurrentCam();

		float4x4 matrix = t->GetLocalTransform().Transposed();


		if(parent_t != nullptr)
		{
			matrix = parent_t->GetGlobalTranform() * t->GetLocalTransform();
			matrix.Transpose();
		}

		// EDIT TRANSFORM QITH GUIZMO
		ImGuizmo::Manipulate(camera->GetViewMatrix().ptr(), camera->GetProjectionMatrix().ptr(), operation, transform_mode, matrix.ptr());

		// Only edit transforms with guizmo if it's selected first
		if (ImGuizmo::IsUsing())
		{
			matrix.Transpose();

			if (selected_object->parent != nullptr)
			{
				matrix = parent_t->GetGlobalTranform().Inverted() * matrix;
			}

			float3 position, scale;
			Quat rotation;
			matrix.Decompose(position, rotation, scale);

			t->SetTranslation(position);
			t->SetRotation(rotation);
			t->SetScale(scale);
		}
	}
}



