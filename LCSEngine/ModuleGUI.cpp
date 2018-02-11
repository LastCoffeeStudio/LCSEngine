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
#include "windows.h"
#include "SDL_assert.h"
#include "SDL\include\SDL.h"
#include "ModuleInput.h"
#include <string> 
#include <shellapi.h>
#include "Brofiler.h"
#include "ComponentFactory.h"

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

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0;
	style.FrameRounding = 2;
	style.GrabRounding = 2;
	style.ItemSpacing.y = 7;

	//ImGui::PushStyleColor(ImGuiCol_TitleBgActive, { 0.2f, 0.2f, 0.22f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_Header, { 0.2f, 0.2f, 0.22f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.2f, 0.2f, 0.22f, 1.f });

	ImGui::PushStyleColor(ImGuiCol_FrameBg, { 0.1f, 0.12f, 0.13f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_CloseButton, { 0.1f, 0.12f, 0.13f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_TitleBg, { 0.1f, 0.12f, 0.13f, 1.f });

	ImGui::PushStyleColor(ImGuiCol_Border, { 0.1f, 0.12f, 0.13f, 1.f });

	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, { 0.28f, 0.3f, 0.32f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, { 0.28f, 0.3f, 0.32f, 1.f });

	ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, { 0.f, 0.f, 0.32f, 1.f });

	io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto-Regular.ttf", 14.0f);

	return true;
}

update_status ModuleGUI::preUpdate(float deltaTime)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	return UPDATE_CONTINUE;
}

update_status ModuleGUI::update(float deltaTime)
{
	BROFILER_CATEGORY("UpdateGUI", Profiler::Color::Orchid)
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
		ImGui::SetNextWindowSize(ImVec2((float)(App->window->width / SCREEN_COLUMNS), (float)(App->window->height - MENU_TOP_BAR_HEIGHT)));
		ImGui::SetNextWindowPos(ImVec2((float)(App->window->width - (App->window->width / SCREEN_COLUMNS)), (float)MENU_TOP_BAR_HEIGHT), ImGuiSetCond_Always);
		showInspector();
	}

	if (show_hierarchy)
	{
		ImGui::SetNextWindowSize(ImVec2((float)(App->window->width / SCREEN_COLUMNS), (float)(App->window->height - MENU_TOP_BAR_HEIGHT - (float)(App->window->height / SCREEN_ROWS))));
		ImGui::SetNextWindowPos(ImVec2(0, MENU_TOP_BAR_HEIGHT), ImGuiSetCond_Always);
		showHierarchy();
	}

	if (show_console)
	{
		ImGui::SetNextWindowSize(ImVec2((float)(App->window->width / SCREEN_COLUMNS) * 4.f, (float)(App->window->height / SCREEN_ROWS)));
		ImGui::SetNextWindowPos(ImVec2(0.f, (float)((App->window->height / SCREEN_ROWS) * (SCREEN_ROWS - 1))), ImGuiSetCond_Always);
		showConsole();
	}

	if (show_static_popup)
	{
		ImGui::SetNextWindowSize(ImVec2((float)(App->window->width / SCREEN_COLUMNS) * 2, (float)(App->window->height / SCREEN_ROWS)));
		ImGui::SetNextWindowPos(ImVec2((App->window->width / 2) - (((float)(App->window->width / SCREEN_COLUMNS))), (App->window->height / 2) - (((float)(App->window->height / SCREEN_ROWS))/2)), ImGuiSetCond_Always);
		showStaticChildernPopUp();
	}

	return UPDATE_CONTINUE;
}

update_status ModuleGUI::postUpdate(float deltaTime)
{
	return UPDATE_CONTINUE;
}

bool ModuleGUI::cleanUp()
{
	ImGui::PopStyleColor(8);
	return false;
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
	//ImGui::SetNextWindowSize(ImVec2((float)App->window->width, (float)MENU_TOP_BAR_HEIGHT));
	//ImGui::Begin("Render Window", &show_main_window, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar);

	// Menu
	if (ImGui::BeginMainMenuBar())
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
		if ((ImGui::BeginMenu("Window")))
		{
			ImGui::MenuItem("Inspector", "Ctrl+I", &show_inspector);
			ImGui::MenuItem("Hierarchy", "Ctrl+H", &show_hierarchy);
			ImGui::MenuItem("Console", "Ctrl+C", &show_console);
			ImGui::MenuItem("Demo Window", nullptr, &show_demo_window);

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (App->input->getKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT && App->input->getKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		show_inspector = !show_inspector;
	}
	if (App->input->getKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT && App->input->getKey(SDL_SCANCODE_H) == KEY_DOWN)
	{
		show_hierarchy = !show_hierarchy;
	}
	if (App->input->getKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT && App->input->getKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		show_console = !show_console;
	}

	//ImGui::End();
}

void ModuleGUI::showInspector()
{
	ImGui::Begin("Inspector", &show_inspector, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse );
	
	if (App->sceneMain->currentObject->parent != nullptr)
	{
		App->sceneMain->currentObject->drawComponentsGui();
	}

	ImGui::End();
}

void ModuleGUI::showStaticChildernPopUp()
{
	ImGui::Begin("Static GameObject", &show_static_popup, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::Text("Do you want to enable/disable the static flags for all the child objects as well?");
	ImGui::Text("");
	if (ImGui::Button("Yes, change children"))
	{
		App->sceneMain->currentObject->setStaticValueToChildrens();
		show_static_popup = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("No, this object only"))
	{
		show_static_popup = false;
	}
	ImGui::End();
}

void ModuleGUI::showHierarchy()
{
	if (ImGui::Begin("Hierarchy", &show_hierarchy, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
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
		color = ImVec4(0.52f, 0.8f, 0.98f, 1.f);
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
	ImGui::PopStyleColor(1);
	
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
	ImGui::Begin("Console", &show_console, ImGuiWindowFlags_NoResize);
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