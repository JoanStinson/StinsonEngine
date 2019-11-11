#include "MSTimer.h"
#include <SDL.h>

MSTimer::MSTimer() {
}

void MSTimer::Start() {
	currentTime = 0;
	runTimer = true;
}

void MSTimer::Read() {
	if (runTimer) 
		currentTime = SDL_GetTicks();
}

int MSTimer::Stop() {
	runTimer = false;
	return currentTime;
}
