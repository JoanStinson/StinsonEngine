#include "ModuleWindow.h"
#include "Globals.h"
#include "Application.h"
#include <SDL.h>
#include <SDL_image.h>

bool ModuleWindow::Init() {
	LOG("Init Module Window\n");
	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else {
		// Create window
		SDL_DisplayMode DM;
		SDL_GetCurrentDisplayMode(0, &DM);
		desktopWidth = DM.w;
		desktopHeight = DM.h;
		width = desktopWidth / 2;
		height = desktopHeight / 2;
		Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL;

		if (FULLSCREEN) 
			flags |= SDL_WINDOW_FULLSCREEN;
		if (BORDERLESS)
			flags |= SDL_WINDOW_BORDERLESS;
		if (RESIZABLE)
			flags |= SDL_WINDOW_RESIZABLE;
		if (FULLSCREEN_DESKTOP)
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		
		if (window == nullptr) {
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else {
			// Get window surface
			SDL_SetWindowResizable(window, SDL_TRUE);
			screen_surface = SDL_GetWindowSurface(window);

			// Set window icon
			SDL_Surface* iconSurface = IMG_Load("../Resources/Assets/Sprites/WindowIcon.png");
			if (iconSurface == nullptr) {
				LOG("Could not load surface with path: %s. IMG_Load: %s", "../Resources/Assets/Sprites/WindowIcon.png", IMG_GetError());
			}
			else {
				SDL_SetWindowIcon(window, iconSurface);
				SDL_FreeSurface(iconSurface);
			}
		}
	}
	return ret;
}

bool ModuleWindow::CleanUp() {
	LOG("Destroying SDL window and quitting all SDL systems\n");
	//Destroy window
	if (window != nullptr) {
		SDL_DestroyWindow(window);
		SDL_FreeSurface(screen_surface);
	}
	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

int ModuleWindow::GetWidth() const {
	return width;
}

int ModuleWindow::GetHeight() const {
	return height;
}

int ModuleWindow::GetDesktopWidth() const {
	return desktopWidth;
}

int ModuleWindow::GetDesktopHeight() const {
	return desktopHeight;
}

SDL_Window& ModuleWindow::GetWindow() const {
	return *window;
}

void ModuleWindow::SetWindowTitle(char *title) {
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::SetFullscreen(bool fullscreen) {
	fullscreen ? SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) : SDL_SetWindowFullscreen(window, 0);
}

void ModuleWindow::SetResizable(bool resizable) {
	resizable ? SDL_SetWindowResizable(window, SDL_TRUE) : SDL_SetWindowResizable(window, SDL_FALSE);
}

void ModuleWindow::SetBorderless(bool borderless) {
	borderless ? SDL_SetWindowBordered(window, SDL_FALSE) : SDL_SetWindowBordered(window, SDL_TRUE);
}

void ModuleWindow::SetFullDesktop(bool fulldesktop) {
	fulldesktop ? SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) : SDL_SetWindowFullscreen(window, 0);
}

void ModuleWindow::SetWindowBrightness(float brightness) {
	SDL_SetWindowBrightness(window, brightness);
}

void ModuleWindow::SetWidth(int newWidth) {
	width = newWidth;
}

void ModuleWindow::SetHeight(int newHeight) {
	height = newHeight;
}

void ModuleWindow::SetWindowSize(int newWidth, int newHeight) {
	SDL_SetWindowSize(window, newWidth, newHeight);
}
