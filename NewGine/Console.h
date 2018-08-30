#pragma once

#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "Imgui\imgui.h"

class Console
{

public:

	Console();
	~Console();

	void Print(const char* text);
	void ConsoleEditor();
	void SetActive(bool active);
	

private:

	ImGuiTextBuffer buffer;
	bool active = false;

};

extern Console* console;

#endif

