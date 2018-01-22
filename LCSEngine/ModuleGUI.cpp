#include "Globals.h"
#include "Application.h"
#include "ModuleGUI.h"
#include "ModuleWindow.h"
#include "imgui-1.53/imgui_impl_sdl_gl3.h"
#include "imgui-1.53/gl3w/GL/gl3w.h"


ModuleGUI::ModuleGUI() {}

ModuleGUI::~ModuleGUI() {}

bool ModuleGUI::init()
{
	gl3wInit(); 

	// Setup ImGui binding
	ImGui_ImplSdlGL3_Init(App->window->window);

	// Setup style
	ImGui::StyleColorsClassic();

	clear_color = ImVec4(0.f, 0.f, 0.f, 1.00f);

	return true;
}

update_status ModuleGUI::preUpdate(const float deltaTime)
{
	glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
	return UPDATE_CONTINUE;
}

update_status ModuleGUI::update(const float deltaTime)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);

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
	ImGui::Render();
	return UPDATE_CONTINUE;
}

bool ModuleGUI::cleanUp()
{
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}

void ModuleGUI::showAboutWindow()
{
	ImGui::SetNextWindowSize(ImVec2(500, 1000), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("About", &show_engine_about);
	ImGui::Text("LCSEngine v1.0.45");
	ImGui::NewLine();
	ImGui::TextWrapped("3D Engine made for a project using OpenGL");
	ImGui::TextWrapped("Made by Carlos Alamo, Ferran Coma, Dani Mateu, Adrian Meizoso and Daniel Romero");
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

	ImGui::TextWrapped(
		"Copyright(C) 2018  Carlos Alamo, Ferran Coma, Dani Mateu, Adrian Meizoso and Daniel Romero\n\n"
		"This program is free software : you can redistribute it and/or modify "
		"it under the terms of the GNU General Public License as published by"
		"the Free Software Foundation, either version 3 of the License, or"
		"(at your option) any later version.\n\n"
		"This program is distributed in the hope that it will be useful,"
		"but WITHOUT ANY WARRANTY; without even the implied warranty of"
		"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the"
		"GNU General Public License for more details.\n\n"
		"You should have received a copy of the GNU General Public License"
		"along with this program.If not, see http://www.gnu.org/licenses/");


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
