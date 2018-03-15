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

FontData* ModuleType::renderFont( const char * text, const char * path, SDL_Color color)
{
	if (fonts.count(path))
	{
		AssetFont* fontAsset = fonts[path];
		TTF_Font * fontTtf = fontAsset->font;
		SDL_Surface *surface = TTF_RenderUTF8_Blended(fontAsset->font, text, color);
		SDL_Surface *flipSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);
		
		if (SDL_MUSTLOCK(surface))
		{
			SDL_LockSurface(surface);
		}

		Uint32 *pixels = (Uint32 *)surface->pixels;
		Uint32 *pixelsFlip = (Uint32 *)flipSurface->pixels;
		for (int x = 0; x < flipSurface->w; x++)
		{
			for (int y = 0, ry = flipSurface->h - 1; y < flipSurface->h; y++, ry--)
			{
				Uint32 pixel = ((Uint32 *)surface->pixels)[(y * surface->w) + x];
				pixelsFlip[(ry * flipSurface->w) + x] = pixel;
			}
		}

		if (SDL_MUSTLOCK(surface))
		{
			SDL_UnlockSurface(surface);
		}

		FontData* fontData = new FontData();
		fontData->width = flipSurface->w;
		fontData->height = flipSurface->h;
		fontData->data = flipSurface->pixels;
		fontData->colors = flipSurface->format->BytesPerPixel;

		return fontData;
	} 
	else
	{
		return nullptr;
	}
}


