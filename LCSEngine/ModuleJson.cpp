#include "Globals.h"
#include "Application.h"
#include "ModuleJson.h"
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

ModuleJson::ModuleJson() {}

ModuleJson::~ModuleJson() {}

string ModuleJson::readPathImage(string path)
{
	ifstream i("file.json");
	json j;
	i >> j;

	return j["path"];
}
