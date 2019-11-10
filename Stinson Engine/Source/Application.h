#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Module.h"
#include <list>

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleRenderExercise;
class ModuleProgram;
class ModuleUI;
class ModuleTextures;
class ModuleCamera;
//class ModuleModelLoader;

class Application {
public:
	Application();
	~Application();

	bool Init();
	UpdateStatus Update();
	bool CleanUp();

public:
	//TODO cleanup vbos, pointers, use functions, passing const and reference, const for functions 
	// IMPORTANT DETAILS! :) #roadtorockstar

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleProgram* programs = nullptr;
	ModuleUI* interfaces = nullptr;
	ModuleTextures* textures = nullptr;
	ModuleCamera* camera = nullptr;
	//ModuleModelLoader* model = nullptr;

private:
	std::list<Module*> modules;
};

extern Application* App;

#endif // __APPLICATION_H__