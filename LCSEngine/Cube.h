#ifndef __CUBE_H__
#define __CUBE_H__

#include "Glew/include/glew.h"
#include <vector>
using namespace std;

enum RenderMode
{
	DIRECTMODE,
	VBO,
	VERTEXARRAY
};

class Cube
{
public:
	Cube();
	Cube(float lengthX, float lengthY, float lengthZ);
	~Cube();

	void draw();
	void changeRenderMode(bool left);
	void initializeValues();
	bool cleanUp();

public:
	float lengthX, lengthY, lengthZ;
	RenderMode renderMode;

private:
	void drawDirectMode();
	void drawVBO();
	void drawVertexArray();

private:
	GLuint idVertVBO, idColVBO, idVertVA, idIndVA, idColVA;
	vector<float> verticesVBO;
	vector<float> colorsVBO;
	vector<float> verticesVA;
	vector<unsigned int> indicesVA;
	vector<float> colorsVA;
};

#endif //__CUBE_H__