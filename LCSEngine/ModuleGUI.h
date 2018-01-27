#ifndef __MODULEGUI_H__
#define __MODULEGUI_H__

#include "Module.h"
#include "Imgui/imgui.h"

class ModuleGUI : public Module
{

public:
	ModuleGUI();
	~ModuleGUI();

	bool init() override;
	update_status preUpdate(float deltaTime) override;
	update_status update(float deltaTime) override;
	update_status postUpdate(float deltaTime) override;

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
	void showConsole();

	void putHyperlink(const char* link);

private:
};

#endif // __MODULEIGUI_H__
