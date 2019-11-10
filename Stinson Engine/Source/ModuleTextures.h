#ifndef __MODULETEXTURES_H__
#define __MODULETEXTURES_H__

#include "Module.h"

typedef unsigned int GLuint;
typedef unsigned int ILenum;
struct ILinfo;

class ModuleTextures : public Module {
public:
	ModuleTextures(){}
	~ModuleTextures(){}

	bool Init() override;
	bool CleanUp() override;

public:
	GLuint Load(const char *file, ILinfo *imageInfo);
	GLuint Load(const char *file, ILinfo *imageInfo, GLuint wrap, bool filterNearest, bool useMipMaps);
};

#endif // __MODULETEXTURES_H__