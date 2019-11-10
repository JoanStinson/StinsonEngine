#ifndef __MODULE_H__
#define __MODULE_H__

#include "Globals.h"

class Module {
public:
	Module(){}

	virtual bool Init() {
		return true;
	}

	virtual bool Start() {
		return true;
	}

	virtual UpdateStatus PreUpdate() {
		return UpdateStatus::CONTINUE;
	}

	virtual UpdateStatus Update() {
		return UpdateStatus::CONTINUE;
	}

	virtual UpdateStatus PostUpdate() {
		return UpdateStatus::CONTINUE;
	}

	virtual bool CleanUp() {
		return true;
	}
};

#endif // __MODULE_H__