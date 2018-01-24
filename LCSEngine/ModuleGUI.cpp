#include "Globals.h"
#include "Application.h"
#include "ModuleGUI.h"
#include "ModuleWindow.h"
#include "Imgui/imgui_impl_sdl_gl3.h"
#include "windows.h"
#include <shellapi.h>


ModuleGUI::ModuleGUI() {}

ModuleGUI::~ModuleGUI() {}

bool ModuleGUI::init()
{
	// Setup ImGui binding
	ImGui_ImplSdlGL3_Init(App->window->window);

	return true;
}

update_status ModuleGUI::preUpdate(const float deltaTime)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	return UPDATE_CONTINUE;
}

update_status ModuleGUI::update(const float deltaTime)
{
	showMainWindow();
	
	if (show_engine_about)
	{
	showAboutWindow();
	}

	if (show_demo_window)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow();
	}
	
	return UPDATE_CONTINUE;
}

update_status ModuleGUI::postUpdate(const float deltaTime)
{
	return UPDATE_CONTINUE;
}

void ModuleGUI::draw()
{
	ImGui::Render();
}

void ModuleGUI::showAboutWindow()
{
	ImVec4 linkColor = ImVec4(0.f, 0.f, 0.f, 1.00f);

	ImGui::SetNextWindowSize(ImVec2(500, 1000), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("About", &show_engine_about);
	ImGui::Text("LCSEngine v1.0.45");
	ImGui::TextWrapped("3D Engine made for a project using OpenGL");
	ImGui::NewLine();

	ImGui::Text("Made by:");
	ImGui::Text("Carlos Alamo");
	putHyperlink("https://github.com/carlos2380");
	ImGui::Text("Ferran Coma");
	putHyperlink("https://github.com/vandalo");
	ImGui::Text("Dani Mateu");
	putHyperlink("https://github.com/Nyghor");
	ImGui::Text("Adrian Meizoso");
	putHyperlink("https://github.com/AdrianMeizoso");
	ImGui::Text("Daniel Romero");
	putHyperlink("https://github.com/lordmizel");
	ImGui::NewLine();

	ImGui::Text("3rd party libraries used:");
	ImGui::BulletText("SDL 2.0.6");
	ImGui::BulletText("SDL Mixer 2.0.6");
	ImGui::BulletText("Glew 2.0.0");
	ImGui::BulletText("Imgui %s", ImGui::GetVersion());
	ImGui::BulletText("MathGeoLib 1.5");
	ImGui::BulletText("OpenGL 3.1");
	ImGui::NewLine();

	ImGui::Text("License:");
	ImGui::Text("GNU GENERAL PUBLIC LICENSE");

	const char* s = "Copyright(C) 2018  Carlos Alamo, Ferran Coma, Dani Mateu, Adrian Meizoso and Daniel Romero\n\n"
		"This program is free software : you can redistribute it and/or modify "
		"it under the terms of the GNU General Public License as published by"
		"the Free Software Foundation, either version 3 of the License, or"
		"(at your option) any later version.\n\n"
		"This program is distributed in the hope that it will be useful,"
		"but WITHOUT ANY WARRANTY; without even the implied warranty of"
		"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the"
		"GNU General Public License for more details.\n\n"
		"You should have received a copy of the GNU General Public License"
		"along with this program.If not, see http://www.gnu.org/licenses/";

	ImGui::TextWrapped(s);

	ImGui::End();
}

void ModuleGUI::showMainWindow()
{
	ImGui::SetNextWindowSize(ImVec2(500, 200), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Render Window", &show_main_window, ImGuiWindowFlags_MenuBar);
	ImGui::Text("Hello, world!");
	ImGui::ColorEdit3("clear color", (float*)&clear_color);
	if (ImGui::Button("Demo Window"))
		show_demo_window ^= 1;
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	// Menu
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About LCSEngine")) {
				show_engine_about ^= 1;
			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::End();
}

void ModuleGUI::putHyperlink(const char * link)
{
	ImGui::TextColored(ImVec4(0.2f, 0.5f, 0.73f, 1.00f), link);
	ImGui::SameLine(1);
	if (ImGui::InvisibleButton(link, ImVec2(ImGui::CalcTextSize(link).x, ImGui::CalcTextSize(link).y)))
	{
		ShellExecute(NULL, TEXT("open"), TEXT(link), NULL, NULL, 0);
	};
}
