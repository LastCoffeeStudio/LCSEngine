#ifndef __MODULEJSON_H__
#define __MODULEJSON_H__

#include "Module.h"
#include <string>

class ModuleJson : public Module
{
public:
	ModuleJson();
	~ModuleJson();

	std::string readPathImage(const std::string path) const;

};

#endif // __MODULEJSON_H__