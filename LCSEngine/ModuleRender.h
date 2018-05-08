#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include "MathGeoLib/src/Math/float4x4.h"
#include <map>
#include <list>

typedef unsigned int GLuint;
typedef unsigned int GLenum;
typedef int GLint;

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

struct renderData
{
	float4x4 id;
	float4x4 normalId;
	GLint idVertVBO;
	GLuint sizeVertVBO;
	GLint idNormalVBO;
	GLint idIdxVAO;
	GLuint sizeNormalVBO;
	GLuint sizeIdxVAO;
	GLuint textureID;	//This has to be a vector later, and meshComponent needs a map: for each mesh added to the verticesVBO, a reference to the textureID that needs
	GLuint normalMapID;
	GLuint textureCoordsID;
	GLuint colorID;
	bool hasMaterial;
	bool hasNormalMap;
	bool hasTexture;
	GLenum mode;
};

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool init();
	update_status preUpdate(float deltaTime);
	update_status update(float deltaTime);
	update_status postUpdate(float deltaTime);
	bool cleanUp();

	void  updatedWindowSize(int screenWidth, int screenHeight);

public:
	SDL_Renderer* renderer = nullptr;
	bool wireframe = false;
	std::multimap<GLuint, renderData> renderQueue;

private:
	void swap();
	void renderObjects();
};

#endif // __MODULERENDER_H__