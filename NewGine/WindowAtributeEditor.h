#pragma once

#include "Window.h"
#include <vector>


class WindowAtributeEditor : public Window
{
public:

	WindowAtributeEditor(Application* app);
	~WindowAtributeEditor();

	void DrawOnEditor();

	void AddComponentWindow();


private:
	
	std::vector<std::string> material_names;
	std::vector<std::string> mesh_names;
};
