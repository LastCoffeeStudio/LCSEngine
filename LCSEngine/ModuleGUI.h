#ifndef __MODULEGUI_H__
#define __MODULEGUI_H__

#include "Module.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl_gl3.h"
#include "MathGeoLib\src\Math\float4x4.h"

typedef float GLfloat;
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
	bool show_animation_window = false;
	bool show_console = true;
	bool show_gl = true;
	bool show_hierarchy = true;
	bool show_static_popup = false;
	bool show_save_popup = false;
	bool show_load_popup = false;
	bool show_audio_settings = false;
	bool show_play_pause = true;
	ImVec4 clear_color;

private:
	void showAboutWindow();
	void showMainWindow();
	void showInspector();
	void showFlagOptions();
	void showConsole();
	void showHierarchy();
	void showStaticChildernPopUp();
	void showSavePopUp();
	void showLoadPopUp();
	void showHierarchyChildren(GameObject* gameObject, bool enabled);
	void showGizmo();
	void showAudioSettings();
	void showAnimationWindow();
	void showPlayPause();

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
	float audio_volume = 1;
	float audio_rolloff = 1;
	float audio_doppler = 1;
	char animationName[128] = "";
	char animationPath[128] = "";
	std::string fileName = "";
	
};

#endif //__MODULEGUI_H__
