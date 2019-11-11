#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleProgram.h"
#include "ModuleUI.h"
#include "ModuleTextures.h"
#include "ModuleCamera.h"
#include "ModuleModelLoader.h"
#include "MSTimer.h"
#include <SDL.h>

MSTimer timer;

Application::Application() {
	timer.Start();
	timer.Read();
	// Order matters: they will Init/start/update in this order
	modules.push_back(window = new ModuleWindow());//TODO make_shared pointer
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(programs = new ModuleProgram());
	modules.push_back(model = new ModuleModelLoader());
	modules.push_back(input = new ModuleInput());
	modules.push_back(interfaces = new ModuleUI());
	LOG("%dms\n", timer.Stop());
}

Application::~Application() {
	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		delete *it;
}

bool Application::Init() {
	timer.Start();
	timer.Read();
	bool ret = true;

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Start();
	
	LOG("%dms\n", timer.Stop());
	return ret;
}

UpdateStatus Application::Update() {
	static bool hasUpdatedOneFrame = false;
	if (!hasUpdatedOneFrame) {
		timer.Start();
		timer.Read();
	}
	UpdateStatus ret = UpdateStatus::CONTINUE;

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UpdateStatus::CONTINUE; ++it)
		ret = (*it)->PreUpdate();

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UpdateStatus::CONTINUE; ++it)
		ret = (*it)->Update();

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UpdateStatus::CONTINUE; ++it)
		ret = (*it)->PostUpdate();

	if (!hasUpdatedOneFrame) {
		LOG("%dms\n", timer.Stop());
		hasUpdatedOneFrame = true;
	}

	return ret;
}

bool Application::CleanUp() {
	timer.Start();
	timer.Read();
	bool ret = true;

	for (std::list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();
	LOG("%dms\n", timer.Stop());
	return ret;
}
