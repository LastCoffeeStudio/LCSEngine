#ifndef __MODULESCENEMAIN_H__
#define __MODULESCENEMAIN_H__

#include "Module.h"

class Shader;
class AssetTexture;
class CubeShape;
class SphereShape;

enum PolygonType
{
	TRIANGLE,
	CUBE,
	SPHERE
};

class ModuleSceneMain : public Module
{
public:
	ModuleSceneMain(bool active = true);
	~ModuleSceneMain();

	bool init() override;
	bool start() override;
	update_status update(float deltaTime) override;
	bool cleanUp() override;
	void draw();
	void drawGrid();

public:
	AssetTexture * checkers = nullptr;
	AssetTexture* lenna = nullptr;
	AssetTexture* chocobo = nullptr;
	AssetTexture* beer = nullptr;
	AssetTexture* actual = nullptr;
	Shader* shader = nullptr;

private:
	PolygonType actualPolygon = TRIANGLE;
	CubeShape* cube1 = nullptr;
	SphereShape* sphere1 = nullptr;
};
#endif //__MODULESCENEMAIN_H__
