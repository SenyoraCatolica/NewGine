#pragma once
#include "JSON\parson.h"
#include <string>


class Application;

class Module
{
private :
	bool enabled;

public:
	
	std::string name;


public:
	Application* App;

	Module(Application* parent, bool start_enabled = true) : App(parent)
	{}

	virtual ~Module()
	{}

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update()
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual bool LoadConfig(JSON_Object* data)
	{
		return true;
	}

	virtual bool SaveConfig(JSON_Object* data) const
	{
		return true;
	}
};