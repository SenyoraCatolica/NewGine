#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

enum WINDOW_MODE
{
	RESIZABLE,
	FULL_DESKTOP,
	FULLSCREEN
};

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);
	int GetWindowMode();
	void SetWindowMode(int type);
	int GetWidth();
	void SetWidth(int width);
	int GetHeight();
	void SetHeight(int height);

	bool LoadConfig(JSON_Object* data);
	bool SaveConfig(JSON_Object* data)const;

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

private:
	int width;
	int height;
	WINDOW_MODE win_mode;
	const char* title = TITLE;
};

#endif // __ModuleWindow_H__