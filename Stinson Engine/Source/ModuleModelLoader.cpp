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
	ILinfo tempinfo;
	activeModel = new Model ("../Resources/Assets/Models/BakerHouse.fbx", App->textures->Load("../Resources/Assets/Textures/BakerHouse.png", &tempinfo), *App->programs->textureProgram);
	activeModel->imageInfo = tempinfo;

	ILinfo checkersImage;
	ILinfo bakerImage;
	ILinfo marioImage;
	ILinfo luigiImage;

	textures.push_back(std::pair<unsigned int, ILinfo>(App->textures->Load("../Resources/Assets/Textures/Checkers.jpg", &checkersImage), checkersImage));
	textures.push_back(std::pair<unsigned int, ILinfo>(App->textures->Load("../Resources/Assets/Textures/BakerHouse.png", &bakerImage), bakerImage));
	textures.push_back(std::pair<unsigned int, ILinfo>(App->textures->Load("../Resources/Assets/Textures/Mario.dds", &marioImage), marioImage));
	textures.push_back(std::pair<unsigned int, ILinfo>(App->textures->Load("../Resources/Assets/Textures/Luigi.dds", &luigiImage), luigiImage));
	return true;
}

bool ModuleModelLoader::CleanUp() {
	delete activeModel;
	return true;
}

Model& ModuleModelLoader::GetActiveModel() const {
	return *activeModel;
}

void ModuleModelLoader::SetActiveModel(const char *filename) {
	activeModel = new Model(filename, activeModel->texture, activeModel->program);
	App->camera->CalculateMatrixes();
	App->camera->Focus();
}

void ModuleModelLoader::SetActiveTexture(unsigned int texture) {
	activeModel->texture = texture;
}

void ModuleModelLoader::SetActiveImageInfo(ILinfo imageInfo) {
	activeModel->imageInfo = imageInfo;
}

void ModuleModelLoader::RenderAllModels() {
	activeModel->Render();
}
