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

void ModuleModelLoader::HandleDroppedFile(const char *droppedFile) {
	std::string modelName = std::string(droppedFile).substr(std::string(droppedFile).length() - 5);

	if (modelName == "e.fbx") {
		App->camera->center = math::float3(0.F, 1.75F, 6.5F);
		App->modelLoader->SetActiveModel(droppedFile);
		SearchFBXFile(droppedFile, 21, "BakerHouse.png");
	}
	else if (modelName == "o.fbx") {
		App->camera->center = math::float3(0.F, 0.75F, 2.2F);
		App->modelLoader->SetActiveModel(droppedFile);
		SearchFBXFile(droppedFile, 16, "Mario.dds");
	}
	else if (modelName == "i.fbx") {
		App->camera->center = math::float3(0.F, 0.85F, 2.4F);
		App->modelLoader->SetActiveModel(droppedFile);
		SearchFBXFile(droppedFile, 16, "Luigi.dds");
	}
	else {
		std::string type = std::string(droppedFile).substr(std::string(droppedFile).length() - 3);
		if (type == "dds" || type == "jpg" || type == "png")
			App->modelLoader->SetActiveTexture(App->textures->Load(droppedFile, &App->modelLoader->GetActiveModel().imageInfo));
	}
}

void ModuleModelLoader::SearchFBXFile(const char *droppedFile, int pathOffset, const char *name) {
	std::string path(droppedFile);
	path.erase(path.end() - 3, path.end());

	LOG("Trying to find matching texture on FBX path '%s'\n", path.c_str());
	std::string format = std::string(name).substr(std::string(name).length() - 3);
	path += format;

	if (App->textures->Load(path.c_str()) != NULL)
		App->modelLoader->SetActiveTexture(App->textures->Load(path.c_str(), &App->modelLoader->GetActiveModel().imageInfo));
	else {
		LOG("Trying to find matching texture on 'Textures\\' folder '%s'\n", path.c_str());
		path.erase(path.end() - pathOffset, path.end());

		std::string nameStr = name;
		nameStr.erase(nameStr.end() - 4, nameStr.end());
		path += "Textures\\" + nameStr + "." + format;

		if (App->textures->Load(path.c_str()) != NULL)
			App->modelLoader->SetActiveTexture(App->textures->Load(path.c_str(), &App->modelLoader->GetActiveModel().imageInfo));
	}
}

void ModuleModelLoader::RenderAllModels() {
	activeModel->Render();
}