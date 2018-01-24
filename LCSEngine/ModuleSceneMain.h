#ifndef __MODULESCENEMAIN_H__
#define __MODULESCENEMAIN_H__

#include "Module.h"

class CubeShape;
class SphereShape;

enum Polygon
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
	update_status update(const float deltaTime) override;
	bool cleanUp() override;
	void draw();
	void drawGrid();

private:
	bool manualRotation;
	Polygon actualPolygon;
	CubeShape* cube1;
	SphereShape* sphere1;
};
#endif //__MODULESCENEMAIN_H__
