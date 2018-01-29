#ifndef __MODULEGUI_H__
#define __MODULEGUI_H__

#include "Module.h"
#include "Imgui/imgui.h"
#include "Glew\include\glew.h"

class ModuleGUI : public Module
{

public:
	ModuleGUI();
	~ModuleGUI();

	bool init() override;
	update_status preUpdate(const float deltaTime) override;
	update_status update(const float deltaTime) override;
	update_status postUpdate(const float deltaTime) override;

	void draw();

	bool show_demo_window = false;
	bool show_main_window = true;
	bool show_engine_about = false;
	bool show_inspector = true;
	bool show_console = true;
	bool show_gl = true;
	ImVec4 clear_color;

private:
	void showAboutWindow();
	void showMainWindow();
	void showInspector();
	void showFlagOptions();
	void showConsole();

	void putHyperlink(const char* link);

private:
	bool checkbox_depthTest;
	bool checkbox_cullFace;
	bool checkbox_lighting;
	GLfloat alColor[3] = { 1.0f, 1.0f, 1.0f };
	bool checkbox_colorMaterial;
	bool checkbox_texture2D;
	bool checkbox_fog;
	GLfloat fogColor[3] = { 1.0f, 1.0f, 1.0f };
	GLfloat fogIntensity = 1.0f;
	bool checkbox_aliasing;
	
};

#endif // __MODULEIGUI_H__
