#include "ModuleInput.h"
#include "Application.h"
#include "ModuleUI.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleRender.h"
#include <SDL.h>

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
	LOG("Init SDL input event system\n");
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

	mouseMotion = { 0, 0 };
	memset(windowEvents, false, (int)EventWindow::COUNT * sizeof(bool));

	const Uint8* keys = SDL_GetKeyboardState(nullptr);

	for (int i = 0; i < MAX_KEYS; ++i) {
		if (keys[i] == 1) {
			if (keyboard[i] == KeyState::IDLE)
				keyboard[i] = KeyState::DOWN;
			else
				keyboard[i] = KeyState::REPEAT;
		}
		else {
			if (keyboard[i] == KeyState::REPEAT || keyboard[i] == KeyState::DOWN)
				keyboard[i] = KeyState::UP;
			else
				keyboard[i] = KeyState::IDLE;
		}
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
