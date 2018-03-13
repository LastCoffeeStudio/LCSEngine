#ifndef __MODULEGAMEUI_H__
#define __MODULEGAMEUI_H__

#include "Module.h"
#include <vector>

using namespace std;

class ElementGameUI;

class ModuleGameUI : public Module
{
public:
	ModuleGameUI();
	~ModuleGameUI();
	update_status update(float deltaTime) override;
	

public:
	vector<ElementGameUI*> elements;

private:
	void printGameUI();

};

#endif // __MODULEGAMEUI_H__