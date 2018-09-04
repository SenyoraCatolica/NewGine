#pragma once
#include "Module.h"
#include "Globals.h"
#include "GameObject.h"

#include "Window.h"
#include "WindowConfig.h"
#include "WindowHardware.h"
#include "WindowAtributeEditor.h"

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

	bool LoadConfig(JSON_Object* data);
	bool SaveConfig(JSON_Object* data) const;

private:

	vector<Window*> windows;
	WindowConfig* configwindow;
	WindowHardware* hardwarewindow;
	WindowAtributeEditor* atributeeditorwindow;

	bool config_active = true;
	bool hardware_active = true;
	bool atributeeditor_active = true;

};
