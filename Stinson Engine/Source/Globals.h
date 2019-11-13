#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <windows.h>
#include <stdio.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_SIZE 1
#define FULLSCREEN false
#define VSYNC true
#define TITLE "Stinson Engine"
#define LOG(format, ...) Log(__FILE__, __LINE__, format, __VA_ARGS__);

void Log(const char file[], int line, const char *format, ...);
void DrawConsoleLogWindow(bool *p_open);

enum class UpdateStatus {
	CONTINUE = 1,
	STOP,
	FAIL
};

#endif // __GLOBALS_H__