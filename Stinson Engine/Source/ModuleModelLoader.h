#ifndef __MODULEMODELLOADER_H__
#define __MODULEMODELLOADER_H__

#include "Module.h"
#include "Model.h"
#include "../Libraries/MathGeoLib/MathGeoLib.h"
#include <vector>
#include <il.h>
#include <ilu.h>
#include <ilut.h>

class ModuleModelLoader : public Module {
public:
	ModuleModelLoader();
	~ModuleModelLoader();

	bool Init() override;
	bool CleanUp() override;

public:
	Model& GetActiveModel() const;

	void SetActiveModel(const char *filename);
	void SetActiveTexture(unsigned int texture);
	void SetActiveImageInfo(ILinfo imageInfo);
	void HandleDroppedFile(const char *droppedFile);
	void SearchFBXFile(const char *droppedFile, int pathOffset, const char *name);
	void RenderAllModels();

public:
	std::vector<std::pair<unsigned int, ILinfo>> textures;

private:
	Model* activeModel = nullptr;
	std::vector<Model*> models;
};

#endif // __MODULEMODELLOADER_H__