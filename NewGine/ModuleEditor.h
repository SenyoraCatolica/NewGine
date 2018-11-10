#pragma once
#include "Module.h"
#include "Globals.h"
#include "GameObject.h"

#include "Window.h"
#include "WindowConfig.h"
#include "WindowHardware.h"
#include "WindowAtributeEditor.h"
#include "WindowConsole.h"
#include "WindowAssets.h"
#include "OutlinerWindow.h"

class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Start();
	update_status Update();
	bool CleanUp();


	bool HandleMainMenu();
	void HandleScenesMenu();
	void InitWindows();
	void GamePanel();
	WindowAssets* GetAssetsWindow();

	bool LoadConfig(JSON_Object* data);
	bool SaveConfig(JSON_Object* data) const;

private:

	vector<Window*> windows;
	WindowConfig* configwindow;
	WindowHardware* hardwarewindow;
	WindowAtributeEditor* atributeeditorwindow;
	WindowAssets* assetswindow;
	OutlinerWindow* outlinerwindow;

	bool config_active;
	bool atributeeditor_active;
	bool console_active;
	bool demo_active;
	bool assets_active;
	bool outliner_active;

	//Scene management
	char scene_name[256] = "";
	bool want_new_scene = false;
	bool want_to_save = false;
	bool want_to_load = false;

public:
	GameObject* selected_object = nullptr;

};
