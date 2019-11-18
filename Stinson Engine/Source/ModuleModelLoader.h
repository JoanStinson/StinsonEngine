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
	void ChangeMesh(const char *filename);
	void ChangeTexture(unsigned int texture, bool saveTexture = true);
	void RenderAllMeshes();

public:
	Model* activeMesh = nullptr;
	ILinfo* activeTexture = nullptr;
	unsigned int previousTexture;
	std::vector<unsigned int> textures;
	std::vector<Model*> models;
};

#endif // __MODULEMODELLOADER_H__