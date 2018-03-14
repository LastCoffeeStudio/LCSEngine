#include "Globals.h"
#include "ModuleType.h"
#include "SDL/include/SDL.h"
#include "Glew/include/glew.h"
#include <iostream>

ModuleType::ModuleType(){}

ModuleType::~ModuleType(){}

bool ModuleType::init()
{
	/*
	FT_Error error = FT_Init_FreeType(&library);
	if (error)
	{
		LOG("Cannot load Freetype library");
		return false;
	}
	*/

	if (TTF_Init() < 0) {
		LOG("Cannot load Freetype library");
		return false;
	}

	const SDL_version *linked_version = TTF_Linked_Version();
	SDL_version compiled_version;
	SDL_TTF_VERSION(&compiled_version);

	LOG("Linked version SDL_TTF: %u.%u.%u", linked_version->major, linked_version->minor, linked_version->patch);
	LOG("Compiled version SDL_TTF: %u.%u.%u", compiled_version.major, compiled_version.minor, compiled_version.patch);

	return true;
}

bool ModuleType::cleanUp()
{
	TTF_Quit();
	return true;
}

update_status ModuleType::update(float deltaTime)
{
	return UPDATE_CONTINUE;
}

void ModuleType::loadFont(const char * path)
{
	AssetFont* fontAsset = new AssetFont();
	fontAsset->font = TTF_OpenFont(path, 28);
	if (fontAsset->font == nullptr) {
		LOG("Error: %s", TTF_GetError());
	}
	else
	{
		fonts[path] = fontAsset;
	}
}

FontData* ModuleType::renderFont( const char * text, const char * path)
{
	if (fonts.count(path))
	{
		AssetFont* fontAsset = fonts[path];
		TTF_Font * fontTtf = fontAsset->font;
		SDL_Surface *surface = TTF_RenderUTF8_Blended(fontAsset->font, text, { 255, 255 ,255 });

		FontData* fontData = new FontData();
		fontData->width = surface->w;
		fontData->height = surface->h;
		fontData->data = surface->pixels;
		fontData->colors = surface->format->BytesPerPixel;

		return fontData;
	} 
	else
	{
		return nullptr;
	}
}


