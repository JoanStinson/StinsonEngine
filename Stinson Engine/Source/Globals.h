#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <windows.h>
#include <stdio.h>

// Configuration -----------
#define FULLSCREEN false
#define RESIZABLE true
#define BORDERLESS false
#define FULLSCREEN_DESKTOP false
#define VSYNC true
#define TITLE "Stinson Engine"
#define LOG(format, ...) Log(__FILE__, __LINE__, format, __VA_ARGS__);

enum class UpdateStatus {
	CONTINUE = 1,
	STOP,
	FAIL
};

void Log(const char file[], int line, const char *format, ...);
void DrawConsoleLogWindow(bool *p_open);

#endif // __GLOBALS_H__