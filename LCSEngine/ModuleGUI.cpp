#include "Globals.h"
#include "Application.h"
#include "ModuleGUI.h"
#include "ModuleWindow.h"
#include "AssetTexture.h"
#include "ModuleSceneMain.h"
#include "Imgui/imgui_impl_sdl_gl3.h"
#include "windows.h"
#include "ModuleCamera.h"
#include <shellapi.h>
#include "ModuleRender.h"
#include <string> 

using namespace std;

ModuleGUI::ModuleGUI() {}

ModuleGUI::~ModuleGUI() {}

bool ModuleGUI::init()
{
	// Setup ImGui binding
	ImGui_ImplSdlGL3_Init(App->window->window);

	checkbox_depthTest = glIsEnabled(GL_DEPTH_TEST);
	checkbox_cullFace = glIsEnabled(GL_CULL_FACE);
	checkbox_lighting = glIsEnabled(GL_LIGHTING);
	checkbox_colorMaterial = glIsEnabled(GL_COLOR_MATERIAL);
	checkbox_texture2D = glIsEnabled(GL_TEXTURE_2D);
	checkbox_fog = glIsEnabled(GL_FOG);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, fogIntensity);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, alColor);
	checkbox_aliasing = glIsEnabled(GL_HISTOGRAM);

	return true;
}

update_status ModuleGUI::preUpdate(float deltaTime)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	return UPDATE_CONTINUE;
}

update_status ModuleGUI::update(float deltaTime)
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
	
	if (show_inspector)
	{
		ImGui::SetNextWindowPos(ImVec2((App->window->width/ SCREEN_COLUMNS) * (SCREEN_COLUMNS-1), 0), ImGuiSetCond_FirstUseEver);
		showInspector();
	}

	if (show_console)
	{
		ImGui::SetNextWindowPos(ImVec2(0, (App->window->height / SCREEN_ROWS) * (SCREEN_ROWS - 1)), ImGuiSetCond_FirstUseEver);
		showConsole();
	}
	return UPDATE_CONTINUE;
}

update_status ModuleGUI::postUpdate(float deltaTime)
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

	// Menu
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About LCSEngine"))
			{
				show_engine_about ^= 1;
			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (ImGui::CollapsingHeader("Camera"))
	{
		ImGui::SliderFloat("FOV Horizontal", &App->camera->frustum.horizontalFov, 0.001f, 3.0f);
		ImGui::SliderFloat("FOV Vertical", &App->camera->frustum.verticalFov, 0.001f, 3.0f);
		ImGui::SliderFloat("Near Plane Distance", &App->camera->frustum.nearPlaneDistance, 0.001f, 100.0f);
		ImGui::SliderFloat("Far Plane Distance", &App->camera->frustum.farPlaneDistance, 0.001f, 100.0f);
		ImGui::SliderFloat("Movement Speed", &App->camera->cameraSpeed, 0.001f, 20.0f);
		ImGui::SliderFloat("Rotation Speed", &App->camera->rotationSpeed, 0.001f, 20.0f);
		ImGui::SliderFloat("Zoom Speed", &App->camera->zoomSpeed, 0.001f, 50.0f);
		//ImGui::SliderInt("Aspect Ratio", &App->window->height, 0.001f, 20.0f);
		ImGui::SliderFloat3("Front", (float*)&App->camera->frustum.front, -10.0f, 10.0f);
		ImGui::SliderFloat3("Up", (float*)&App->camera->frustum.up, -10.0f, 10.0f);
		ImGui::SliderFloat3("Position", (float*)&App->camera->frustum.pos, -10.0f, 10.0f);
		ImGui::ColorEdit3("Background Color", (float*)&clear_color);
		ImGui::Checkbox("Frustum culling", &App->camera->frustumCulling);
		ImGui::Checkbox("Wireframe Mode", &App->renderer->wireframe);
		if (ImGui::Button("Demo Window"))
			show_demo_window ^= 1;
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		
	}
	ImGui::End();
}

void ModuleGUI::showInspector()
{
	ImGui::SetNextWindowSize(ImVec2((App->window->width / SCREEN_COLUMNS), App->window->height));
	ImGui::Begin("Inspector", &show_inspector, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Inspector");
	//Set functions to print diferent menus inside the inspector
	if (ImGui::CollapsingHeader("Textures"))
	{
		static int e = 0;
		ImGui::RadioButton("radio a", &e, 0); ImGui::SameLine();
		ImGui::RadioButton("radio b", &e, 1); ImGui::SameLine();
		ImGui::RadioButton("radio c", &e, 2);

		//Code to show data from textures depending in which one is taken
		switch (e)
		{
			case 0:
				App->sceneMain->actual = App->sceneMain->lenna;
				break;
			case 1:
				App->sceneMain->actual = App->sceneMain->beer;
				break;
			case 2:
				App->sceneMain->actual = App->sceneMain->chocobo;
				break;
		}
	}

	//Show current texture
	AssetTexture* current = App->sceneMain->actual;

	ImGui::Text("Path"); ImGui::SameLine(0); ImGui::Text(current->name.c_str());
	ImGui::Text("Width"); ImGui::SameLine(0); ImGui::Text(to_string(current->width).c_str());
	ImGui::Text("Height"); ImGui::SameLine(0); ImGui::Text(to_string(current->height).c_str());
	ImGui::Text("Depth"); ImGui::SameLine(0); ImGui::Text(to_string(current->depth).c_str());

	showFlagOptions();					///////

	ImGui::End();
}

void ModuleGUI::showFlagOptions()
{
	if (ImGui::CollapsingHeader("OpenGL Flags"))
	{
		if (ImGui::Checkbox("Depth test", &checkbox_depthTest))
		{
			checkbox_depthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
		}
		if (ImGui::Checkbox("Cull face", &checkbox_cullFace))
		{
			checkbox_cullFace ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
		}
		if (ImGui::Checkbox("Lighting", &checkbox_lighting))
		{
			checkbox_lighting ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
		}
		if (glIsEnabled(GL_LIGHTING))
		{
			if (ImGui::SliderFloat3("Lighting color", alColor, 0.0f, 1.0f))
			{
				glLightModelfv(GL_LIGHT_MODEL_AMBIENT, alColor);
			}
			if (ImGui::Checkbox("Color material", &checkbox_colorMaterial))
			{
				checkbox_colorMaterial ? glEnable(GL_COLOR_MATERIAL) : glDisable(GL_COLOR_MATERIAL);
			}
		}
		if (ImGui::Checkbox("Texture 2D", &checkbox_texture2D))
		{
			checkbox_texture2D ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);
		}
		if (ImGui::Checkbox("Fog", &checkbox_fog))
		{
			checkbox_fog ? glEnable(GL_FOG) : glDisable(GL_FOG);
		}
		if (glIsEnabled(GL_FOG))
		{
			if (ImGui::SliderFloat3("Fog color", fogColor, 0.0f, 1.0f))
			{
				glFogfv(GL_FOG_COLOR, fogColor);
			}
			if (ImGui::SliderFloat("Fog Intensity", &fogIntensity, 0.0f, 1.0f))
			{
				glFogf(GL_FOG_DENSITY, fogIntensity);
			}
		}
		if (ImGui::Checkbox("Aliasing", &checkbox_aliasing))
		{
			checkbox_aliasing ? glEnable(GL_HISTOGRAM) : glDisable(GL_HISTOGRAM);
		}
	}
}

void ModuleGUI::showConsole()
{
	ImGui::SetNextWindowSize(ImVec2((App->window->width/ SCREEN_COLUMNS) * 4, App->window->height/SCREEN_ROWS));

	ImGui::Begin("Console", &show_console, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Console");

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