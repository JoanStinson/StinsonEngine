#ifndef __MODULEUI_H__
#define __MODULEUI_H__

#include "Module.h"
#include "../Libraries/ImGui/imgui.h"
#include <vector>

union SDL_Event;
//TODO have an UI for each model to display texture and model settings
//TODO imgui texture of camera to window, use frame buffer
class ModuleUI : public Module {
public:
	ModuleUI() {}
	~ModuleUI() {}

	bool Init() override;
	bool Start() override;
	UpdateStatus PreUpdate() override;
	UpdateStatus Update() override;
	UpdateStatus PostUpdate() override;
	bool CleanUp() override;

public:
	void ProcessEvent(const SDL_Event *event) const;

private:
	UpdateStatus DrawMainBar();
	void DrawConfigWindow(bool *p_open);
	void DrawPropertiesWindow(bool *p_open);
	void DrawAboutWindow(bool *p_open);

	void DrawCameraHeader();
	void DrawHardwareHeader();
	void DrawPerformanceHeader();
	void DrawTexturesHeader();
	void DrawWindowHeader();

	void DrawLogFrames(std::vector<float> &frames, float value, char *titleBuf, int titleSize, const char *title, const char *histogramTitle, float maxHeight, unsigned int &time);
	void HelpMarker(const char *desc, bool showText = true) const;
	void RequestBrowser(const char *link) const;

private:
	ImGuiIO io;

	bool showConsoleLogWindow = true;
	bool showConfigWindow = true;
	bool showPropertiesWindow = false;
	bool showDemoWindow = false;
	bool showAboutWindow = false;
	bool fullscreen = false;
	bool resizable = true;
	bool borderless = false;
	bool fullDesktop = false;

	bool showTriangle = false;
	bool showSquare = true;
	bool showSanicTexture = false;
	bool showButterfliesTexture = true;
	bool showLinkTexture = false;
	bool showRepeat = false;
	bool showMirroredRepeat = false;
	bool showClampToEdge = false;
	bool showClampToBorder = true;
	bool showNearest = false;
	bool showLinear = true;
	bool showMipMaps = true;

	char engineName[40] = TITLE;
	char version[12] = "1.0.0";
	const char* description = "A 3D engine using C++ with OpenGL that tries\n"
		"to mimic Unity's functionalities. It's a project\n"
		"developed for the master's degree of Advanced\n"
		"Programming for AAA Video Games at the UPC\n"
		"School (Polytechnic University of Catalonia).";
	char author[25] = "Joan Ginard";
	char fpsTitle[24];
	char msTitle[24];

	float brightness = 1.0f;
	unsigned int currentTime;
	unsigned int fpsTime = 0;
	unsigned int msTime = 0;
	int width = SCREEN_WIDTH;
	int height = SCREEN_HEIGHT;

	std::vector<float> fpsLog;
	std::vector<float> msLog;
	std::string caps;
};

#endif