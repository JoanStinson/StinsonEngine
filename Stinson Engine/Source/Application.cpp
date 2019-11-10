#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleProgram.h"
#include "ModuleUI.h"
#include "ModuleTextures.h"
#include "ModuleCamera.h"
//#include "ModuleModelLoader.h"

Application::Application() {
	// Order matters: they will Init/start/update in this order
	modules.push_back(window = new ModuleWindow());//TODO make_shared pointer
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(camera = new ModuleCamera());
	//modules.push_back(model = new ModuleModelLoader());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(programs = new ModuleProgram());
	modules.push_back(input = new ModuleInput());
	modules.push_back(interfaces = new ModuleUI());
}

Application::~Application() {
	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		delete *it;
}

bool Application::Init() {
	bool ret = true;

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Start();

	return ret;
}

UpdateStatus Application::Update() {
	UpdateStatus ret = UpdateStatus::CONTINUE;

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UpdateStatus::CONTINUE; ++it)
		ret = (*it)->PreUpdate();

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UpdateStatus::CONTINUE; ++it)
		ret = (*it)->Update();

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UpdateStatus::CONTINUE; ++it)
		ret = (*it)->PostUpdate();

	return ret;
}

bool Application::CleanUp() {
	bool ret = true;

	for (std::list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}
