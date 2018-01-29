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
	float radius = 1.f;
	int nSeg = 8;

private:
	void drawDirectMode();

private:
	unsigned int totalCoordsVBO = 0;
	unsigned int totalTexCoordsVBO = 0;
};

#endif //__SPHERESHAPE_H__