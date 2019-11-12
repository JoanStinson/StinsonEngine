#include "ModuleModelLoader.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleTextures.h"

ModuleModelLoader::ModuleModelLoader() {
}

ModuleModelLoader::~ModuleModelLoader() {
}

bool ModuleModelLoader::Init() {
	LOG("Init Model Loader\n");

	Mesh *bakerHouse = new Mesh ("../Resources/Assets/Models/BakerHouse.fbx", App->textures->Load("../Resources/Assets/Textures/BakerHouse.dds"), *App->programs->textureProgram);
	meshes.push_back(bakerHouse);

	Mesh *mario = new Mesh("../Resources/Assets/Models/Mario.fbx", App->textures->Load("../Resources/Assets/Textures/Mario.dds"), *App->programs->textureProgram);
	meshes.push_back(mario);
	return true;
}

bool ModuleModelLoader::CleanUp() {
	for (int i = 0; i < meshes.size(); ++i)
		delete meshes.at(i);
	meshes.clear();
	return true;
}

void ModuleModelLoader::ChangeMesh(int index, const char *filename, unsigned texture, unsigned program) {
	meshes[index] = new Mesh(filename, texture, program);
}

void ModuleModelLoader::RenderAllMeshes() {
	for (int i = 0; i < meshes.size(); ++i)
		meshes[i]->Render();
}
