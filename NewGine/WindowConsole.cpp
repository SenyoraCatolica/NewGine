#include "WindowConsole.h"
#include "Imgui\imgui.h"
#include "Globals.h"
#include "Application.h"

WindowConsole::WindowConsole(Application* app) : Window(app) {}

WindowConsole::~WindowConsole()
{
}


void WindowConsole::DrawOnEditor()
{

	if(active)
	{
		ImGui::SetWindowSize(ImVec2(1000, 300));
		ImGui::SetWindowPos(ImVec2(SCREEN_WIDTH - 1390, SCREEN_HEIGHT - 310));

		ImGui::TextWrapped("This is the console for our engine, it will display all information that is relevant for internal operations");
		ImGui::TextWrapped("Enter 'HELP' for help");

		if (ImGui::SmallButton("Add Dummy Text"))
		{
			LOG("1 some text");
			LOG("some more text");
			LOG("display very important message here!");

		} ImGui::SameLine();

		if (ImGui::SmallButton("Add Dummy Error"))
			LOG("[error] something went wrong"); ImGui::SameLine();

		if (ImGui::SmallButton("Clear"))
			App->console->ClearLog(); ImGui::SameLine();

		if (ImGui::SmallButton("Scroll to bottom"))
			ScrollToBottom = true;

		ImGui::Separator();

		for (std::list<ConsoleLog*>::iterator it = App->console->messages.begin; it != App->console->messages.end; ++it)
		{
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), it._Ptr->_Myval->data);
		}
	}
}