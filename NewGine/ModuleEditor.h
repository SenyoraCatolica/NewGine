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
	update_status Update(float dt);
	bool CleanUp();


	bool HandleMainMenu();
	void InitWindows();
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

public:
	GameObject* selected_object;

};
