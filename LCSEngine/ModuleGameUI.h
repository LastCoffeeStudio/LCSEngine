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
	void printGameUI();

public:
	vector<ElementGameUI*> elements;
};

#endif // __MODULEGAMEUI_H__