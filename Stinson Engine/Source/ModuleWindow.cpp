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
		int width = SCREEN_WIDTH;
		int height = SCREEN_HEIGHT;
		Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL;

		if (FULLSCREEN) {
			flags |= SDL_WINDOW_FULLSCREEN;
		}

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
			SDL_Surface* iconSurface = IMG_Load("../Resources/Assets/iconWindow.png");
			if (iconSurface == nullptr) {
				LOG("Could not load surface with path: %s. IMG_Load: %s", "../Resources/Assets/iconWindow.png", IMG_GetError());
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

void ModuleWindow::SetWindowSize(int width, int height) {
	SDL_SetWindowSize(window, width, height);
}

int ModuleWindow::GetWindowWidth() const {
	int w;
	int h;
	SDL_GetWindowSize(window, &w, &h);
	return w;
}

int ModuleWindow::GetWindowHeight() const {
	int w;
	int h;
	SDL_GetWindowSize(window, &w, &h);
	return h;
}
