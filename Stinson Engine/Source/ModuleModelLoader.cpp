#include "ModuleModelLoader.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleTextures.h"
#include "ModuleCamera.h"

ModuleModelLoader::ModuleModelLoader() {
}

ModuleModelLoader::~ModuleModelLoader() {
}

bool ModuleModelLoader::Init() {
	LOG("Init Model Loader\n");
	activeTexture = new ILinfo();
	activeMesh = new Mesh ("../Resources/Assets/Models/BakerHouse.fbx", App->textures->Load("../Resources/Assets/Textures/BakerHouse.png", activeTexture), *App->programs->textureProgram);
	previousTexture = activeMesh->texture;
	return true;
}

bool ModuleModelLoader::CleanUp() {
	delete activeMesh;
	return true;
}

void ModuleModelLoader::ChangeMesh(const char *filename) {
	activeMesh = new Mesh(filename, activeMesh->texture, activeMesh->program);
	App->camera->CalculateMatrixes();
	App->camera->Focus();
}

void ModuleModelLoader::ChangeTexture(unsigned int texture, bool saveTexture) {
	activeMesh->texture = texture;
	if (saveTexture)
		previousTexture = texture;
}

void ModuleModelLoader::RenderAllMeshes() {
	activeMesh->Render();
}
