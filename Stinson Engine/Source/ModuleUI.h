#ifndef __MODULEUI_H__
#define __MODULEUI_H__

#include "Module.h"
#include "../Libraries/ImGui/imgui.h"
#include <vector>

#define RED ImVec4(1.0F, 0.0F, 0.0F, 1.0F)
#define GREEN ImVec4(0.0F, 1.0F, 0.0F, 1.0F)
#define BLUE ImVec4(0.0F, 0.0F, 1.0F, 1.0F)
#define PINK ImVec4(1.0F, 0.0F, 1.0F, 1.0F)
#define YELLOW ImVec4(1.0F, 1.0F, 0.0F, 1.0F)
#define LIGHT_BLUE ImVec4(0.0F, 1.0F, 1.0F, 1.0F)

union SDL_Event;

class ModuleUI : public Module {
public:
	ModuleUI(){}
	~ModuleUI(){}

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
	void DrawAboutWindow(bool *p_open);
	void DrawLogFrames(std::vector<float> &frames, float value, char *titleBuf, int titleSize, const char *title, const char *histogramTitle, float maxHeight, unsigned int &time);
	void HelpMarker(const char *desc, bool showText = true) const;
	void RequestBrowser(const char *link) const;

private:
	ImGuiIO io;

	char engineName[40] = TITLE;
	char description[140] = "3D engine for UPC master's degree.";
	char author[25] = "Joan Ginard";
	char fpsTitle[24];
	char msTitle[24];

	bool showConsoleLogWindow = false;
	bool showConfigWindow = true;
	bool showDemoWindow = false;
	bool showAboutWindow = false;
	bool fullscreen = false;
	bool resizable = true;
	bool borderless = false;
	bool fullDesktop = false;

	float brightness = 1.0f;
	unsigned int currentTime;
	unsigned int fpsTime = 0;
	unsigned int msTime = 0;
	int width = SCREEN_WIDTH;
	int height = SCREEN_HEIGHT;

	std::vector<float> fpsLog;
	std::vector<float> msLog;
	std::string caps;

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
};

#endif