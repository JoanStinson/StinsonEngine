#ifndef __MODULEMODELLOADER_H__
#define __MODULEMODELLOADER_H__

#include "Module.h"
#include "Mesh.h"
#include <vector>

enum class MeshModel {
	BAKEHOUSE
};

class ModuleModelLoader : public Module {
public:
	ModuleModelLoader();
	~ModuleModelLoader();

	bool Init() override;
	bool CleanUp() override;

public:
	void ChangeMesh(const char *filename, int index = 0);
	void ChangeTexture(unsigned int texture, int index = 0);
	void RenderAllMeshes();

private:
	std::vector<Mesh*> meshes;
};

#endif // __MODULEMODELLOADER_H__