#pragma once
#include "Module.h"
#include <ft2build.h>
#include <map>

#include FT_FREETYPE_H
typedef struct _TTF_Font TTF_Font;

typedef unsigned char GLubyte;
typedef unsigned int GLuint;
typedef float GLfloat;

struct AssetFont
{
	TTF_Font* font;
};

struct FontData {                           
	GLuint idTexture; 
	int width;
	int height;
	void* data;
	Uint8 colors;
};

class ModuleType :	public Module
{
public:
	ModuleType();
	~ModuleType();

	bool init() override;
	bool cleanUp() override;
	update_status update(float deltaTime) override;
	void loadFont(const char* path);
	FontData* renderFont(const char* text, const char* path);

public:
	FT_Library library;
	std::map<std::string, AssetFont*> fonts;

private:
	void gen_bitmap(FT_Bitmap* bitmap, int posx, int posy);


};

