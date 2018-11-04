#pragma once

#include "Window.h"
#include "GameObject.h"


class OutlinerWindow : public Window
{

public:
	OutlinerWindow(Application* App);
	~OutlinerWindow();

	void DrawOnEditor();

private:

	void ChildHierarchy(GameObject* parent);
};