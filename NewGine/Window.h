#pragma once
#ifndef __WINDOW_H__
#define __WINDOW_H__

class Application;


class Window
{

public:

	Application* App;

	Window(Application* app);

	virtual void DrawOnEditor() {};
	void SetActive(bool value);
	bool GetActive();

	bool active = false;

};
#endif