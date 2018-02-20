#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "AssetTexture.h"
#include "SDL/include/SDL.h"
#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilut.h"
#include <stdlib.h> 
#include <stdio.h> 
#include "SDL_image/include/SDL_image.h"
#include "assimp/include/scene.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

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

	LOG("Init Devil Library");
	ilInit();

	return ret;
}

// Called before quitting
bool ModuleTextures::cleanUp()
{
	return true;
}

AssetTexture* const ModuleTextures::loadTexture(ILenum type, const char* path)
{
	AssetTexture* asset = nullptr;
	ILuint imageID;
	GLuint textureID;
	ILboolean success;
	ILenum error;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);
	success = ilLoad(type,path);

	if (success)
	{
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		if (!success)
		{
			error = ilGetError();
			printf("Image conversion failed - IL reports error: %s\n", iluErrorString(error));
			return asset;
		}

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
		glBindTexture(GL_TEXTURE_2D, 0);

		asset = new AssetTexture(ImageInfo);
		asset->ID = textureID;
		asset->name = path;

		//Delete file from memory
		ilDeleteImages(1, &imageID);
	}
	else
	{
		error = ilGetError();
		printf("Image load failed - IL reports error: %s\n", iluErrorString(error));
		return asset;
	}

	ilDeleteImages(1, &imageID);
	
	return asset;
}

void const ModuleTextures::loadModelTextures(const aiScene* scene, std::map<unsigned int, AssetTexture*>& textures)
{
	ILboolean success;
	ILenum error;
	//Fill textures with all generated id textures from DevIL
	for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
	{
		aiString path;
		//We suppose only one diffuse per material, so index is 0
		aiReturn texture = scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		
		if (texture == AI_SUCCESS)
		{
			ILuint imageID;
			ilGenImages(1, &imageID);
			ilBindImage(imageID);
			char p[100];
			strcpy_s(p, 100, "Assets/Models/");
			strcat_s(p, 100, path.data);
			//success = ilLoadImage(path.data);
			success = ilLoad(IL_PNG, p);
			if (success)
			{
				ILinfo ImageInfo;
				iluGetImageInfo(&ImageInfo);

				if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
				{
					iluFlipImage();
				}

				ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

				GLuint textureID;
				glGenTextures(1, &textureID);
				glBindTexture(GL_TEXTURE_2D, textureID);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH),
					ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
				glBindTexture(GL_TEXTURE_2D, 0);

				AssetTexture* asset = new AssetTexture(ImageInfo);
				asset->ID = textureID;
				asset->name = path.data;

				textures[i] = asset;
			}
			else
			{
				error = ilGetError();
				printf("Image load failed - IL reports error: %s\n", iluErrorString(error));
			}
		}
	}
}

AssetTexture* const ModuleTextures::loadCheckers()
{
	AssetTexture* checkers = new AssetTexture();
	checkers->height = CHECKERS_HEIGHT;
	checkers->width = CHECKERS_WIDTH;
	checkers->bpp = 1;
	checkers->depth = 4;
	checkers->mips = 0;
	checkers->bytes = sizeof(GLubyte) * CHECKERS_HEIGHT * CHECKERS_WIDTH * 4;

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
	glGenTextures(1, &checkers->ID);
	glBindTexture(GL_TEXTURE_2D, checkers->ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	glBindTexture(GL_TEXTURE_2D, 0);

	return checkers;
}