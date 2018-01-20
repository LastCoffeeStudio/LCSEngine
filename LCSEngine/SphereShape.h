#ifndef __SPHERESHAPE_H__
#define __SPHERESHAPE_H__

#include "Primitive.h"
#include "Glew/include/glew.h"
#include <vector>
using namespace std;

class SphereShape : public Primitive
{
public:
	SphereShape();
	SphereShape(float radius, int nSeg);
	~SphereShape();

	void initializeValues();
	bool cleanUp();

public:
	float radius;
	int nSeg;

private:
	void drawDirectMode();

private:
	int totalCoordsVBO;
};

#endif //__SPHERESHAPE_H__