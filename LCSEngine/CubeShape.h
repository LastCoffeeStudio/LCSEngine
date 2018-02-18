#ifndef __CUBESHAPE_H__
#define __CUBESHAPE_H__

#include "Primitive.h"
#include <vector>

using namespace std;

class CubeShape : public Primitive
{
public:
	CubeShape();
	CubeShape(float lengthX, float lengthY, float lengthZ);
	~CubeShape();

	void initializeValues() override;
	bool cleanUp() override;

public:
	float lengthX, lengthY, lengthZ;

private:
	void drawDirectMode() override;
};

#endif //__CUBESHAPE_H__