#ifndef __MODULEPROGRAM_H__
#define __MODULEPROGRAM_H__

#include "Module.h"
#include <string>

class ModuleProgram : public Module {
public:
	ModuleProgram(){}
	~ModuleProgram(){}

	bool Init() override;
	bool CleanUp() override;

public:
	unsigned int def_program;

private:
	void CheckCompileErrors(unsigned int shader, const std::string &type) const;
};

#endif // __MODULEPROGRAM_H__ 