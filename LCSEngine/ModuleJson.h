#ifndef __MODULEJSON_H__
#define __MODULEJSON_H__

#include "Module.h"
#include <string>

using namespace std;

class ModuleJson : public Module
{
public:
	ModuleJson();
	~ModuleJson();

	string readPathImage(string path);
};

#endif // __MODULEJSON_H__