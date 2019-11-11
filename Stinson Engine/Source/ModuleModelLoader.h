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
	void ChangeMesh(int index, const char *filename, unsigned texture, unsigned program);
	void RenderAllMeshes();

private:
	std::vector<Mesh*> meshes;
};

#endif // __MODULEMODELLOADER_H__