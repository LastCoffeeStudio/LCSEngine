#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"
#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilut.h"

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

using namespace std;

ModuleTextures::ModuleTextures() {}

// Destructor
ModuleTextures::~ModuleTextures()
{
	IMG_Quit();
}

// Called before render is available
bool ModuleTextures::init()
{
	LOG("Init Image library");
	bool ret = true;

	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if ((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

// Called before quitting
bool ModuleTextures::cleanUp()
{
	LOG("Freeing textures and Image library");

	for (list<SDL_Texture*>::iterator it = textures.begin(); it != textures.end(); ++it)
		SDL_DestroyTexture(*it);

	textures.clear();
	return true;
}

/*
// Load new texture from file path
SDL_Texture* const ModuleTextures::load(const char* path)
{
	SDL_Texture* texture = nullptr;
	SDL_Surface* surface = IMG_Load(path);

	if (surface == nullptr)
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
	}
	else
	{
		texture = SDL_CreateTextureFromSurface(App->renderer->renderer, surface);

		if (texture == nullptr)
		{
			LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			textures.push_back(texture);
		}

		SDL_FreeSurface(surface);
	}

	return texture;
}*/

// Load new texture from file path
bool const ModuleTextures::load(const char* path)
{
	//Texture loading success
	bool textureLoaded = false;

	//Generate and set current image ID
	ILuint imgID;
	ilGenImages(1, &imgID);
	ilBindImage(imgID);

	//Calculate size of image
	ILubyte *Lump;
	ILuint Size;
	FILE *File;

	fopen_s(&File, path, "rb");
	fseek(File, 0, SEEK_END);
	Size = ftell(File);

	Lump = (ILubyte*)malloc(Size);
	fseek(File, 0, SEEK_SET);
	fread(Lump, 1, Size, File);
	fclose(File);

	//Load image
	if (ilLoadL(IL_JPG, Lump, Size))
	{
		ILinfo info;
		iluGetImageInfo(&info);

			/*
		//Convert image to RGBA
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

			if (success == IL_TRUE)
			{
				//Create texture from file pixels
				//textureLoaded = loadTextureFromPixels32((GLuint*)ilGetData(), (GLuint)ilGetInteger(IL_IMAGE_WIDTH), (GLuint)ilGetInteger(IL_IMAGE_HEIGHT));

				iluGetImageInfo
			}
			*/

		//Delete file from memory
		ilDeleteImages(1, &imgID);
	}

	//Report error
	if (!textureLoaded)
	{
		printf("Unable to load %s\n", path);
	}

	return textureLoaded;
}

void ModuleTextures::loadCheckers()
{
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &ImageName);
	glBindTexture(GL_TEXTURE_2D, ImageName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// Free texture from memory
void ModuleTextures::unload(SDL_Texture* texture)
{
	for (list<SDL_Texture*>::iterator it = textures.begin(); it != textures.end(); ++it)
	{
		if (*it == texture)
		{
			SDL_DestroyTexture(*it);
			textures.erase(it);
			break;
		}
	}
}