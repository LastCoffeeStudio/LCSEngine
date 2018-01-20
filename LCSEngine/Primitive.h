#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include "Glew/include/glew.h"
#include <vector>
using namespace std;

enum RenderMode
{
	DIRECTMODE,
	VBO,
	VERTEXARRAY
};

class Primitive
{
public:
	Primitive();
	~Primitive();

	void draw();
	void changeRenderMode(bool left);
	virtual void initializeValues();
	virtual bool cleanUp();

public:
	RenderMode renderMode;

private:
	virtual void drawDirectMode();
	virtual void drawVBO();
	virtual void drawVertexArray();

protected:
	GLuint idVertVBO, idColVBO, idVertVA, idIndVA, idColVA;
	vector<float> verticesVBO;
	vector<float> colorsVBO;
	vector<float> verticesVA;
	vector<unsigned int> indicesVA;
	vector<float> colorsVA;
};

#endif //__PRIMITIVE_H__