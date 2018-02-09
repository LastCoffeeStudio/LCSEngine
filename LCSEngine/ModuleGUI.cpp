#include "Globals.h"
#include "Application.h"
#include "ModuleGUI.h"
#include "ModuleWindow.h"
#include "ModuleSceneMain.h"
#include "ModuleCamera.h"
#include "ModuleRender.h"
#include "AssetTexture.h"
#include "GameObject.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "Imgui/imgui_impl_sdl_gl3.h"
#include "windows.h"
#include "SDL_assert.h"
#include <string> 
#include <shellapi.h>

ModuleGUI::ModuleGUI() {}

ModuleGUI::~ModuleGUI() {}

bool ModuleGUI::init()
{
	// Setup ImGui binding
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
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
	checkbox_aliasing = glIsEnabled(GL_POLYGON_SMOOTH);

	return true;
}

update_status ModuleGUI::preUpdate(float deltaTime)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	return UPDATE_CONTINUE;
}

update_status ModuleGUI::update(float deltaTime)
{
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);
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
		ImGui::SetNextWindowPos(ImVec2((float)((App->window->width/ SCREEN_COLUMNS) * (SCREEN_COLUMNS-1)), (float)MENU_TOP_BAR_HEIGHT), ImGuiSetCond_Always);
		showInspector();
	}

	if (show_hierarchy)
	{
		ImGui::SetNextWindowPos(ImVec2(0, MENU_TOP_BAR_HEIGHT), ImGuiSetCond_Always);
		showHierarchy();
	}

	if (show_console)
	{
		ImGui::SetNextWindowPos(ImVec2(0.f, (float)((App->window->height / SCREEN_ROWS) * (SCREEN_ROWS - 1))), ImGuiSetCond_Always);
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
	ImGui::SetNextWindowSize(ImVec2((float)App->window->width, (float)MENU_TOP_BAR_HEIGHT));
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
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Print ZBuffer"))
			{
				App->sceneMain->swapDefaultShader();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::End();
}

void ModuleGUI::showInspector()
{
	ImGui::SetNextWindowSize(ImVec2((float)(App->window->width / SCREEN_COLUMNS), (float)(App->window->height - MENU_TOP_BAR_HEIGHT)));
	ImGui::Begin("Inspector", &show_inspector, ImGuiWindowFlags_AlwaysAutoResize);
	
	if (App->sceneMain->currentObject->parent != nullptr)
	{
		App->sceneMain->currentObject->drawComponentsGui();
	}

	ImGui::End();
}

void ModuleGUI::showHierarchy()
{
	ImGui::SetNextWindowSize(ImVec2((float)(App->window->width / SCREEN_COLUMNS), (float)(App->window->height - MENU_TOP_BAR_HEIGHT)));

	if (ImGui::Begin("Hierarchy", &show_hierarchy, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (ImGui::IsMouseHoveringWindow() && ImGui::IsMouseClicked(0))
		{
			App->sceneMain->currentObject = App->sceneMain->root;
		}
		if (ImGui::IsMouseHoveringWindow() &&ImGui::IsMouseClicked(1))
		{
			ImGui::OpenPopup("GameObject Menu");
		}
		if (ImGui::BeginPopup("GameObject Menu"))
		{
			if (ImGui::MenuItem("New GameObject"))
			{
				App->sceneMain->currentObject->addGameObject(new GameObject(App->sceneMain->currentObject, "GameObject"));
			}
			if (ImGui::MenuItem("Delete GameObject"))
			{
				if (App->sceneMain->currentObject != App->sceneMain->root)
				{
					App->sceneMain->currentObject->suicide = true;
					App->sceneMain->currentObject = App->sceneMain->root;
				}
			}
			ImGui::EndPopup();
		}
		//Set functions to print diferent menus inside the inspector
		GameObject* root = App->sceneMain->root;

		for (vector<GameObject*>::iterator it = root->children.begin(); it != root->children.end(); ++it)
		{
			showHierarchyChildren((*it), true);
		}

		ImGui::End();
	}
}

void ModuleGUI::showHierarchyChildren(GameObject* gameObject, bool enabled)
{
	SDL_assert(gameObject != nullptr);
	ImGuiTreeNodeFlags node_flags = 0;
	ImVec4 color = ImVec4(1.f,1.f,1.f,1.f);
	
	if (App->sceneMain->currentObject == gameObject)
	{
		node_flags |= ImGuiTreeNodeFlags_Selected;
	}
	if (gameObject->children.size() > 0)
	{
		node_flags |= ImGuiTreeNodeFlags_OpenOnArrow;
	}
	else
	{
		node_flags |= ImGuiTreeNodeFlags_Leaf;
	}
	if ((enabled == true && gameObject->enable == false) || enabled == false)
	{
		enabled = false;
		color = ImVec4(0.5f, 0.5f, 0.5f, 1.f);
	}

	ImGui::PushStyleColor(ImGuiCol_Text, color);
	bool node = ImGui::TreeNodeEx(gameObject->name.c_str(), node_flags);
	ImGui::PopStyleColor();
	
	if (node)
	{
		if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1))
		{
			App->sceneMain->currentObject = gameObject;
		}
		if (ImGui::IsItemClicked(1))
		{
			ImGui::OpenPopup("GameObject Menu");
		}
		if (ImGui::BeginPopup("GameObject Menu"))
		{
			if (ImGui::MenuItem("New GameObject"))
			{
				App->sceneMain->currentObject->addGameObject(new GameObject(App->sceneMain->currentObject, "GameObject"));
			}
			if (ImGui::MenuItem("Delete GameObject"))
			{
				if (App->sceneMain->currentObject != App->sceneMain->root)
				{
					App->sceneMain->currentObject->suicide = true;
					App->sceneMain->currentObject = App->sceneMain->root;
				}
			}
			ImGui::EndPopup();
		}
		for (vector<GameObject*>::iterator it = gameObject->children.begin(); it != gameObject->children.end(); ++it)
		{
			showHierarchyChildren((*it), enabled);
		}
		ImGui::TreePop();
	}
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
			checkbox_aliasing ? glEnable(GL_POLYGON_SMOOTH) : glDisable(GL_POLYGON_SMOOTH);
		}
	}
}

void ModuleGUI::showConsole()
{
	ImGui::SetNextWindowSize(ImVec2((float)(App->window->width/ SCREEN_COLUMNS) * 4.f, (float)(App->window->height/SCREEN_ROWS)));
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
	}
}