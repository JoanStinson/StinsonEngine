#include "ModuleModelLoader.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleTextures.h"
#include "ModuleCamera.h"

template <typename T>
bool ElementInVector(const std::vector<T> &vec, const T &element) {
	for (unsigned i = 0; i < vec.size(); ++i)
		if (vec[i] == element) return true;
	return false;
}

ModuleModelLoader::ModuleModelLoader() {
}

ModuleModelLoader::~ModuleModelLoader() {
}

bool ModuleModelLoader::Init() {
	LOG("Init Model Loader\n");
	activeTexture = new ILinfo();
	activeMesh = new Model ("../Resources/Assets/Models/BakerHouse.fbx", App->textures->Load("../Resources/Assets/Textures/BakerHouse.png", activeTexture), *App->programs->textureProgram);
	previousTexture = activeMesh->texture;
	return true;
}

bool ModuleModelLoader::CleanUp() {
	delete activeMesh;
	return true;
}

void ModuleModelLoader::ChangeMesh(const char *filename) {
	activeMesh = new Model(filename, activeMesh->texture, activeMesh->program);
	App->camera->CalculateMatrixes();
	App->camera->Focus();
}

void ModuleModelLoader::ChangeTexture(unsigned int texture, bool saveTexture) {
	if (activeMesh->texture != texture) {
		activeMesh->texture = texture;
		if (saveTexture)
			previousTexture = texture;

		//TODO loop and check if its not repeated
		if (!ElementInVector(textures, texture)) {
			textures.push_back(texture);
		}
	}
}

void ModuleModelLoader::RenderAllMeshes() {
	activeMesh->Render();
}
