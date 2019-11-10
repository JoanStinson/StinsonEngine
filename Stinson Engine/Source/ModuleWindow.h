#ifndef __MODULEWINDOW_H__
#define __MODULEWINDOW_H__

#include "Module.h"

struct SDL_Window;
struct SDL_Surface;

class ModuleWindow : public Module {
public:
	ModuleWindow(){}
	~ModuleWindow(){}

	bool Init() override;
	bool CleanUp() override;

public:
	int GetWindowWidth() const;
	int GetWindowHeight() const;

	void SetWindowTitle(char *title);
	void SetFullscreen(bool fullscreen);
	void SetResizable(bool resizable);
	void SetBorderless(bool borderless);
	void SetFullDesktop(bool fulldesktop);
	void SetWindowBrightness(float brightness);
	void SetWindowSize(int width, int height);

public:
	SDL_Window* window = nullptr;
	SDL_Surface* screen_surface = nullptr;
};

#endif // __MODULEWINDOW_H__