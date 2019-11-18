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
	int GetWidth() const;
	int GetHeight() const;
	int GetDesktopWidth() const;
	int GetDesktopHeight() const;
	SDL_Window& GetWindow() const;

	void SetWindowTitle(char *title);
	void SetFullscreen(bool fullscreen);
	void SetResizable(bool resizable);
	void SetBorderless(bool borderless);
	void SetFullDesktop(bool fulldesktop);
	void SetWindowBrightness(float brightness);
	void SetWidth(int newWidth);
	void SetHeight(int newHeight);
	void SetWindowSize(int newWidth, int newHeight);

private:
	SDL_Window* window = nullptr;
	SDL_Surface* screen_surface = nullptr;
	int width;
	int height;
	int desktopWidth;
	int desktopHeight;
};

#endif // __MODULEWINDOW_H__