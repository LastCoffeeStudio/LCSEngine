#ifndef __MODULEINTRO_H__
#define __MODULEINTRO_H__

#include "Module.h"

class ModuleIntro : public Module
{
public:
	ModuleIntro();
	~ModuleIntro();

	bool Start() override;
	update_status Update(const float deltaTime) override;

public:
	
};
#endif //__MODULEINTRO_H__
