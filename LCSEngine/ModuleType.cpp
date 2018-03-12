#include "ModuleType.h"


ModuleType::ModuleType(){}

ModuleType::~ModuleType(){}

bool ModuleType::init()
{
	FT_Error error = FT_Init_FreeType(&library);
	if (error)
	{
		LOG("Cannot load Freetype library");
	}

	return true;
}

update_status ModuleType::update(float deltaTime)
{
	return UPDATE_CONTINUE;
}

void ModuleType::loadFont(const char * path)
{
	if (fonts.find(path) == fonts.end())
	{
		AssetFont* font = new AssetFont();
		bool error = FT_New_Face(library, path, 0, &font->face);

		if (error == FT_Err_Unknown_File_Format)
		{
			LOG("Cannot load the font, wrong format");
		}
		else if (error)
		{
			LOG("Cannot load font, unknow error");
		}
		else
		{
			fonts[path] = font;
		}
	}
}
