#pragma once
#include "Module.h"
#include <ft2build.h>

#include FT_FREETYPE_H


class ModuleType :	public Module
{
public:
	ModuleType();
	~ModuleType();

	bool init() override;
	update_status update(float deltaTime) override;

public:
	FT_Library library;
};

