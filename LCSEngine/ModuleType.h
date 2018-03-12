#pragma once
#include "Module.h"
#include <ft2build.h>
#include <map>

#include FT_FREETYPE_H

struct AssetFont
{
	FT_Face face;
};

class ModuleType :	public Module
{
public:
	ModuleType();
	~ModuleType();

	bool init() override;
	update_status update(float deltaTime) override;
	void loadFont(const char* path);

public:
	FT_Library library;
	std::map<std::string, AssetFont*> fonts;
};

