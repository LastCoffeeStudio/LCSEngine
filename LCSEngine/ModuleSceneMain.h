#ifndef __MODULESCENEMAIN_H__
#define __MODULESCENEMAIN_H__

#include "Module.h"

class Cube;

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

private:
	Polygon actualPolygon;
	Cube* cube1;
};
#endif //__MODULESCENEMAIN_H__
