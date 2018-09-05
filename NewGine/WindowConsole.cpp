#include "WindowConsole.h"
#include "Imgui\imgui.h"
#include "Globals.h"

WindowConsole::WindowConsole(Application* app) : Window(app) {}

WindowConsole::~WindowConsole()
{
	ClearLog();
}

void WindowConsole::AddLog(const char* log, bool error)
{
	Log* temp = new Log();
	temp->data = log;
	temp->error = error;
	Logs.push_back(temp);
	ScrollToBottom = true;
}

void WindowConsole::ClearLog()
{
	Logs.clear();
	ScrollToBottom = true;
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
			AddLog("1 some text");
			AddLog("some more text");
			AddLog("display very important message here!");

		} ImGui::SameLine();

		if (ImGui::SmallButton("Add Dummy Error"))
			AddLog("[error] something went wrong", true); ImGui::SameLine();

		if (ImGui::SmallButton("Clear"))
			ClearLog(); ImGui::SameLine();

		if (ImGui::SmallButton("Scroll to bottom"))
			ScrollToBottom = true;

		ImGui::Separator();

		for (std::list<Log*>::iterator it = Logs.begin(); it != Logs.end(); ++it)
		{
			if (it._Ptr->_Myval->error)
				ImGui::TextColored(ImColor(1.0f, 0.4f, 0.4f, 1.0f), it._Ptr->_Myval->data);
			else
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), it._Ptr->_Myval->data);
		}
	}
}