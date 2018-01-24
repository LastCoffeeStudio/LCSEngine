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
	update_status preUpdate(const float deltaTime) override;
	update_status update(const float deltaTime) override;
	update_status postUpdate(const float deltaTime) override;

	void draw();

	bool show_demo_window = false;
	bool show_main_window = true;
	bool show_engine_about = false;
	bool show_gl = true;
	ImVec4 clear_color;

private:
	void showAboutWindow();
	void showMainWindow();

	void putHyperlink(const char* link);
};

#endif // __MODULEIGUI_H__
