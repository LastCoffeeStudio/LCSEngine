#include "Globals.h"
#include "Application.h"
#include "ModuleJson.h"
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

ModuleJson::ModuleJson() {}

ModuleJson::~ModuleJson() {}

std::string ModuleJson::readPathImage(const std::string path) const
{
	std::ifstream i(path);
	json j;
	i >> j;

	return j["path"];
}
