#ifndef __MODULEPROGRAM_H__
#define __MODULEPROGRAM_H__

#include "Module.h"

class ModuleProgram : public Module {
public:
	ModuleProgram();
	~ModuleProgram();

	bool Init() override;
	bool CleanUp() override;

public:
	unsigned int *textureProgram = nullptr;
	unsigned int *gridLinesProgram = nullptr;
	unsigned int *screenProgram = nullptr;

private:
	unsigned LoadShader(const char* vertexFile, const char* fragmentFile);
	void CheckCompileErrors(unsigned int shader, const char *type) const;
};

#endif // __MODULEPROGRAM_H__ 