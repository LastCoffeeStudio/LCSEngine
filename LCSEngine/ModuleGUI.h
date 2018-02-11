#ifndef __MODULEGUI_H__
#define __MODULEGUI_H__

#include "Module.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl_gl3.h"
#include "Glew/include/glew.h"

class GameObject;

class ModuleGUI : public Module
{

public:
	ModuleGUI();
	~ModuleGUI();

	bool init() override;
	update_status preUpdate(float deltaTime) override;
	update_status update(float deltaTime) override;
	update_status postUpdate(float deltaTime) override;
	bool cleanUp() override;

	void draw();

	bool show_demo_window = false;
	bool show_main_window = true;
	bool show_engine_about = false;
	bool show_inspector = true;
	bool show_console = true;
	bool show_gl = true;
	bool show_hierarchy = true;
	bool show_static_popup = false;
	ImVec4 clear_color;

private:
	void showAboutWindow();
	void showMainWindow();
	void showInspector();
	void showFlagOptions();
	void showConsole();
	void showHierarchy();
	void showStaticChildernPopUp();
	void showHierarchyChildren(GameObject* gameObject, bool enabled);

	void putHyperlink(const char* link);

private:
	bool checkbox_depthTest = false;
	bool checkbox_cullFace = false;
	bool checkbox_lighting = false;
	GLfloat alColor[3] = { 1.0f, 1.0f, 1.0f };
	bool checkbox_colorMaterial = false;
	bool checkbox_texture2D = false;
	bool checkbox_fog = false;
	GLfloat fogColor[3] = { 1.0f, 1.0f, 1.0f };
	GLfloat fogIntensity = 1.0f;
	bool checkbox_aliasing = false;
	
};

#endif //__MODULEGUI_H__
