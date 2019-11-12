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
	return true;
}

bool ModuleModelLoader::CleanUp() {
	for (int i = 0; i < meshes.size(); ++i)
		delete meshes[i];
	meshes.clear();
	return true;
}

void ModuleModelLoader::ChangeMesh(const char *filename, int index) {
	meshes[index] = new Mesh(filename, meshes[index]->texture, meshes[index]->program);
}

void ModuleModelLoader::ChangeTexture(unsigned texture, int index) {
	meshes[index]->texture = texture;
}

void ModuleModelLoader::RenderAllMeshes() {
	for (int i = 0; i < meshes.size(); ++i)
		meshes[i]->Render();
}
