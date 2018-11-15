#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = SCREEN_WIDTH * SCREEN_SIZE;
		int height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

int ModuleWindow::GetWindowMode()
{
	return win_mode;
}

void ModuleWindow::SetWindowMode(const int type)
{
	switch (type)
	{
	case WINDOW_MODE::RESIZABLE:
	{
		win_mode = RESIZABLE;
		SDL_SetWindowFullscreen(window, SDL_WINDOW_RESIZABLE);
		break;
	}
	case WINDOW_MODE::FULL_DESKTOP:
	{
		win_mode = FULL_DESKTOP;
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		break;
	}
	case WINDOW_MODE::FULLSCREEN:
	{
		win_mode = FULLSCREEN;
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
		break;
	}
	}
}

int ModuleWindow::GetWidth()
{
	return width;
}

void ModuleWindow::SetWidth(int width)
{
	this->width = width;
	SDL_SetWindowSize(window, width, height);
}


int ModuleWindow::GetHeight()
{
	return height;
}

void ModuleWindow::SetHeight(int height)
{
	this->height = height;
	SDL_SetWindowSize(window, width, height);
}

bool ModuleWindow::LoadConfig(JSON_Object* data)
{
	bool ret = true;

	title = json_object_get_string(data, "title");
	width = json_object_get_number(data, "width");
	height = json_object_get_number(data, "height");

	switch ((int)json_object_get_number(data, "window_mode"))
	{
	case WINDOW_MODE::RESIZABLE: win_mode = RESIZABLE; break;
	case WINDOW_MODE::FULL_DESKTOP: win_mode = FULL_DESKTOP; break;
	case WINDOW_MODE::FULLSCREEN: win_mode = FULLSCREEN; break;
	}

	if (width <= 0)width = SCREEN_WIDTH;
	if (height <= 0)height = SCREEN_HEIGHT;

	return ret;
}

bool ModuleWindow::SaveConfig(JSON_Object* data) const
{
	bool ret = true;

	json_object_set_number(data, "width", width);
	json_object_set_number(data, "height", height);
	json_object_set_number(data, "window_mode", (int)win_mode);
	json_object_set_string(data, "title", title);

	return ret;
}