#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include <vector>

using namespace std;

typedef unsigned int GLuint;

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
	RenderMode renderMode = DIRECTMODE;

private:
	virtual void drawDirectMode();
	virtual void drawVBO();
	virtual void drawVertexArray();

protected:
	GLuint idVertVBO = 0;
	GLuint idColVBO = 0;
	GLuint idTexCoordVBO = 0;
	GLuint idVertVA = 0;
	GLuint idIndVA = 0;
	GLuint idColVA = 0;
	
	vector<float> verticesVBO;
	vector<float> colorsVBO;
	vector<float> texcoordsVBO;
	vector<float> verticesVA;
	vector<unsigned int> indicesVA;
	vector<float> colorsVA;
};

#endif //__PRIMITIVE_H__