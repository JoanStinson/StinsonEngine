#ifndef __MODULEINPUT_H__
#define __MODULEINPUT_H__

#include "Module.h"
#include "Point.h"
#include <SDL_scancode.h>
#include "../Libraries/MathGeoLib/MathGeoLib.h"

#define MAX_KEYS 300
#define NUM_MOUSE_BUTTONS 5

typedef unsigned __int8 Uint8;
struct SDL_MouseWheelEvent;

enum class EventWindow {
	QUIT, HIDE, RESIZE, SHOW, COUNT
};

enum class KeyState {
	IDLE, DOWN, REPEAT, UP
};

enum class MouseMove {
	RIGHT, LEFT, UP, DOWN
};

class ModuleInput : public Module {
public:
	ModuleInput();
	~ModuleInput();

	bool Init() override;
	UpdateStatus PreUpdate() override;
	bool CleanUp() override;

public:
	KeyState GetKey(int id) const;
	KeyState GetMouseButtonDown(int id) const;
	bool GetWindowEvent(EventWindow code) const;
	const iPoint& GetMouseMotion() const;
	const iPoint& GetMousePosition() const;
	SDL_MouseWheelEvent* GetMouseWheel() const;

private:
	bool windowEvents[(int)EventWindow::COUNT];
	KeyState* keyboard;
	KeyState mouseButtons[NUM_MOUSE_BUTTONS];
	iPoint mouseMotion;
	iPoint mouse;
	SDL_MouseWheelEvent* mouseWheel = nullptr;
};

#endif // __MODULEINPUT_H__