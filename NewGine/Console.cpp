
#include "Console.h"



Console::Console(){}

Console::~Console()
{
	buffer.clear();
}


void Console::Print(const char* text)
{
	buffer.append(text);
}


void Console::ConsoleEditor()
{

	

	if (active == true)
	{
		ImGui::Begin("Console");

		ImGui::SetWindowSize(ImVec2(100, 100));
		ImGui::SetWindowPos(ImVec2(500, 25));
		ImGui::TextUnformatted(buffer.begin());

		ImGui::End();
	}
}

void Console::SetActive(bool active)
{
	this->active = active;
}