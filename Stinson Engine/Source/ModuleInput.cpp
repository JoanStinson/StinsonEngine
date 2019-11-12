#include "ModuleInput.h"
#include "Application.h"
#include "ModuleUI.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleModelLoader.h"
#include <SDL.h>
#include <string>

#define MAX_KEYS 300

ModuleInput::ModuleInput() : mouseMotion({ 0,0 }), mouse({ 0, 0 }) {
	keyboard = new KeyState[MAX_KEYS];
	memset(keyboard, (int)KeyState::IDLE, sizeof(KeyState) * MAX_KEYS);
	memset(mouseButtons, (int)KeyState::IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);
	mouseWheel = new SDL_MouseWheelEvent();
}

ModuleInput::~ModuleInput() {
	delete keyboard;
}

bool ModuleInput::Init() {
	LOG("Init Module Input\n");
	bool ret = true;
	SDL_Init(0);
	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0) {
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	return ret;
}

UpdateStatus ModuleInput::PreUpdate() {
	static SDL_Event event;

	memset(windowEvents, false, (int)EventWindow::COUNT * sizeof(bool));

	const Uint8* keys = SDL_GetKeyboardState(nullptr);

	for (int i = 0; i < MAX_KEYS; ++i) {
		keys[i] == 1 ?
			(keyboard[i] = (keyboard[i] == KeyState::IDLE) ? KeyState::DOWN : KeyState::REPEAT) :
			(keyboard[i] = (keyboard[i] == KeyState::REPEAT || keyboard[i] == KeyState::DOWN) ? KeyState::UP : KeyState::IDLE);
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i) {
		if (mouseButtons[i] == KeyState::DOWN)
			mouseButtons[i] = KeyState::REPEAT;
		if (mouseButtons[i] == KeyState::UP)
			mouseButtons[i] = KeyState::IDLE;
	}

	while (SDL_PollEvent(&event)) {

		App->interfaces->ProcessEvent(&event);

		switch (event.type) {
		case SDL_QUIT:
			windowEvents[(int)EventWindow::QUIT] = true;
			return UpdateStatus::STOP;
			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event) {
			case SDL_WINDOWEVENT_LEAVE:
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				windowEvents[(int)EventWindow::HIDE] = true;
				break;

			case SDL_WINDOWEVENT_ENTER:
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				windowEvents[(int)EventWindow::SHOW] = true;
				break;

			case SDL_WINDOWEVENT_RESIZED:
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				windowEvents[(int)EventWindow::RESIZE] = true;
				App->camera->SetAspectRatio((float)App->window->GetWindowWidth() / (float)App->window->GetWindowHeight());
				App->renderer->WindowResized(event.window.data1, event.window.data2);
				break;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouseButtons[event.button.button - 1] = KeyState::DOWN;
			break;

		case SDL_MOUSEBUTTONUP:
			mouseButtons[event.button.button - 1] = KeyState::UP;
			break;

		case SDL_MOUSEMOTION:
			mouseMotion.x = event.motion.xrel / SCREEN_SIZE;
			mouseMotion.y = event.motion.yrel / SCREEN_SIZE;
			mouse.x = event.motion.x / SCREEN_SIZE;
			mouse.y = event.motion.y / SCREEN_SIZE;
			break;

		case SDL_MOUSEWHEEL:
			*mouseWheel = event.wheel;
			break;

		case SDL_DROPFILE:
			const char* droppedFile = event.drop.file;
			std::string type = std::string(droppedFile).substr(std::string(droppedFile).length() - 3);
			type == "fbx" ? App->model->ChangeMesh(droppedFile) : 
				type == "dds" || type == "jpg" || type == "png" ? App->model->ChangeTexture(App->textures->Load(droppedFile)) : LOG("Incorrect format of dropped file!\n");
			break;
		}
	}

	if (GetWindowEvent(EventWindow::QUIT) || GetKey(SDL_SCANCODE_ESCAPE) == KeyState::DOWN)
		return UpdateStatus::STOP;

	return UpdateStatus::CONTINUE;
}

bool ModuleInput::CleanUp() {
	LOG("Quitting SDL input event subsystem\n");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

KeyState ModuleInput::GetKey(int id) const {
	return keyboard[id];
}

KeyState ModuleInput::GetMouseButtonDown(int id) const {
	return mouseButtons[id - 1];
}

bool ModuleInput::GetWindowEvent(EventWindow code) const {
	return windowEvents[(int)code];
}

const iPoint& ModuleInput::GetMouseMotion() const {
	return mouseMotion;
}

const iPoint& ModuleInput::GetMousePosition() const {
	return mouse;
}

SDL_MouseWheelEvent* ModuleInput::GetMouseWheel() const {
	return mouseWheel;
}
