#include "Globals.h"
#include "ModuleType.h"
#include "SDL/include/SDL.h"
#include "SDL_ttf/include/SDL_ttf.h"
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
	fontAsset->font = TTF_OpenFont(path, 12);
	if (fontAsset->font == nullptr) {
		TTF_SetError("Loading failed :( (code: %d)", 142);
		LOG("Error: %s", TTF_GetError());
	}
	fonts[path] = fontAsset;

	/*
	SDL_Surface * sFont = TTF_RenderText_Blended(font, message, color);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sFont->w, sFont->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, sFont->pixels);
	*/


	/*
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
			FT_Set_Char_Size(font->face, 12 << 6, 12 << 6, 96, 96);
			fonts[path] = font;
		}
	}
	*/
}

FontData* ModuleType::renderFont( const char * text, const char * path)
{
	AssetFont* fontAsset = fonts[path];
	SDL_Surface *surface = TTF_RenderUTF8_Blended(fontAsset->font, text, {255,0,0,155});

	FontData* fontData = new FontData();
	fontData->width = surface->w;
	fontData->height = surface->h;
	fontData->data = surface->pixels;
	fontData->colors = surface->format->BytesPerPixel;

	return fontData;

	/*
	AssetFont* font = fonts[path];

	FT_Face face = font->face;

	FT_GlyphSlot slot = face->glyph; 
	int pen_x, pen_y;

	pen_x = x;
	pen_y = y;

	int size = sizeof(text) / sizeof(text[0]);

	for (int n = 0; n < size; n++)
	{
		FT_UInt  glyph_index;

		glyph_index = FT_Get_Char_Index(face, text[n]);

		FT_Error error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
		if (error)
		{
			LOG("Cannot load Glyph");
		}

		error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
		if (error)
		{
			LOG("Cannot render Glyph");
		}

		gen_bitmap(&slot->bitmap,
			pen_x + slot->bitmap_left,
			pen_y - slot->bitmap_top);

		pen_x += slot->advance.x >> 6;
		pen_y += slot->advance.y >> 6;
	}

	*/


}

void ModuleType::gen_bitmap(FT_Bitmap* bitmap, int posx, int posy)
{
	/*
	int width = bitmap->width;
	int height = bitmap->rows;

	
	GLubyte* expanded_data = new GLubyte[2 * width * height];
	
	for (int j = 0; j <height; j++) {
		for (int i = 0; i < width; i++) {
			expanded_data[2 * (i + j * width)] = expanded_data[2 * (i + j * width) + 1] =
				(i >= bitmap->width || j >= bitmap->rows) ?
				0 : bitmap->buffer[i + bitmap->width*j];
		}
	}
	

	FontData* data = new FontData();

	glGenBuffers(1, (GLuint*) &(data->idTexture));
	glBindBuffer(GL_ARRAY_BUFFER, data->idTexture);
	glBufferData(GL_ARRAY_BUFFER, sizeof(char) * width * height, bitmap->buffer, GL_STATIC_DRAW);

	//delete[] expanded_data;

	*/
}
