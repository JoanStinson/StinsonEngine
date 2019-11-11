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

	Mesh *bakeHouse = new Mesh ("../Resources/Assets/BakerHouse.fbx", App->textures->Load("../Resources/Assets/BakerHouse.png"), *App->programs->textureProgram);
	meshes.push_back(bakeHouse);

	//Mesh *samus = new Mesh("../Resources/Assets/samus.obj", App->textures->Load("../Resources/Assets/samus.png"), *App->programs->textureProgram);
	//meshes.push_back(samus);
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
