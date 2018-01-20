#include "Globals.h"
#include "SphereShape.h"

SphereShape::SphereShape()
{
	radius = 1.f;
	nSeg = 8;
}

SphereShape::SphereShape(float radius, int nSeg) : radius(radius), nSeg(nSeg)
{
	if (nSeg < 4)
	{
		nSeg = 4;
	}
	if (nSeg > 200)
	{
		nSeg = 200;
	}
}

SphereShape::~SphereShape() {}

void SphereShape::initializeValues()
{
	/*This is divided by parts to understand what we're doing (Just done once, so we don't care about optimization here)*/
	int nRowsQuads = (nSeg - 4) / 2;		//nº rows with quads on the sphere
	int vertRQ = nSeg * 18;					//2 triangles/quad * 3 vertices/triangle * 3 coords/vertex
	int vertSide = nSeg * 9;				//3 vertices/triangle * 3 coords/vertex
	totalCoordsVBO = vertSide * 2 + nRowsQuads * vertRQ;

	verticesVBO.reserve(totalCoordsVBO);
	colorsVBO.reserve(totalCoordsVBO);
	//verticesVA.reserve(24);
	//indicesVA.reserve(36);
	//colorsVA.reserve(24);



	/*float r, g, b;
	r = (rand() / (float)RAND_MAX);
	g = (rand() / (float)RAND_MAX);
	b = (rand() / (float)RAND_MAX);
	for (unsigned int i = 0; i < 36; ++i)
	{
		if (i != 0 && i % 6 == 0)
		{
			r = (rand() / (float)RAND_MAX);
			g = (rand() / (float)RAND_MAX);
			b = (rand() / (float)RAND_MAX);
		}
		colorsVBO.push_back(r);
		colorsVBO.push_back(g);
		colorsVBO.push_back(b);
	}

	verticesVA = { -lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f, lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f,
		lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f, -lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f,
		-lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f, lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f,
		lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f, -lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f };

	indicesVA = { 1, 3, 2, 1, 0, 3,
		0, 7, 3, 0, 4, 7,
		5, 2, 6, 5, 1, 2,
		5, 0, 1, 5, 4, 0,
		2, 7, 6, 2, 3, 7,
		4, 6, 7, 4, 5, 6 };*/

	/** NOTE: If we want each face of one colour instead of assigning each vertex a colour, we must create 24 vertices
	(4 for each face, so the array would be of size 72) instead of 8. That way we can assign a single colour for 4 vertices
	of the same face, and the others as well **/
	/*colorsVA = { 1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 1.f, 1.f,
		0.f, 0.f, 1.f,
		0.f, 1.f, 1.f,
		0.f, 0.f, 1.f,
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f };*/

	/*idVertVBO = 0;	//Assign initial value in case GenBuffers fails
	glGenBuffers(1, (GLuint*) &(idVertVBO));
	glBindBuffer(GL_ARRAY_BUFFER, idVertVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesVBO.size(), &verticesVBO[0], GL_STATIC_DRAW);

	idColVBO = 1;
	glGenBuffers(1, (GLuint*) &(idColVBO));
	glBindBuffer(GL_ARRAY_BUFFER, idColVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colorsVBO.size(), &colorsVBO[0], GL_STATIC_DRAW);

	idVertVA = 2;
	glGenBuffers(1, (GLuint*) &(idVertVA));
	glBindBuffer(GL_ARRAY_BUFFER, idVertVA);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesVA.size(), &verticesVA[0], GL_STATIC_DRAW);

	idIndVA = 3;
	glGenBuffers(1, (GLuint*) &(idIndVA));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndVA);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesVA.size(), &indicesVA[0], GL_STATIC_DRAW);

	idColVA = 4;
	glGenBuffers(1, (GLuint*) &(idColVA));
	glBindBuffer(GL_ARRAY_BUFFER, idColVA);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colorsVA.size(), &colorsVA[0], GL_STATIC_DRAW);*/
}

bool SphereShape::cleanUp()
{
	/*glDeleteBuffers(1, &(idVertVBO));
	glDeleteBuffers(1, &(idColVBO));
	glDeleteBuffers(1, &(idVertVA));
	glDeleteBuffers(1, &(idIndVA));
	glDeleteBuffers(1, &(idColVA));*/

	return true;
}

void SphereShape::drawDirectMode()
{
	/** We fix a pivot on the center of the cube, i.e. we divide by 2 **/

	glBegin(GL_TRIANGLES);

	float difa = 360.f / (float)nSeg;
	float r, g, b;
	r = g = b = 0.3f;
	for (int i = 0; i < nSeg / 2; ++i)
	{
		for (int j = 0; j < nSeg; ++j)
		{
			float sini = sin(i*difa*(float)M_PI / 180.f);
			float sinj = sin(j*difa*(float)M_PI / 180.f);
			float cosi = cos(i*difa*(float)M_PI / 180.f);
			float cosj = cos(j*difa*(float)M_PI / 180.f);
			float sinii = sin((i + 1)*difa*(float)M_PI / 180.f);
			float sinjj = sin((j + 1)*difa*(float)M_PI / 180.f);
			float cosii = cos((i + 1)*difa*(float)M_PI / 180.f);
			float cosjj = cos((j + 1)*difa*(float)M_PI / 180.f);
			if (i == 0)
			{
				glColor3f(r, g, b);
				glVertex3f(sinj*sini*radius, cosi*radius, cosj*sini*radius);
				glVertex3f(sinj*sinii*radius, cosii*radius, cosj*sinii*radius);
				glVertex3f(sinjj*sinii*radius, cosii*radius, cosjj*sinii*radius);
				r += 0.04f;
			}
			else if (i == (nSeg / 2) - 1)
			{
				r = g = 0.f;
				glColor3f(r, g, b);
				glVertex3f(sinjj*sini*radius, cosi*radius, cosjj*sini*radius);
				glVertex3f(sinj*sini*radius, cosi*radius, cosj*sini*radius);
				glVertex3f(sinj*sinii*radius, cosii*radius, cosj*sinii*radius);
				b += 0.04f;
			}
			else
			{
				r = 0.f;
				glColor3f(r, g, b);
				glVertex3f(sinj*sini*radius, cosi*radius, cosj*sini*radius);
				glVertex3f(sinj*sinii*radius, cosii*radius, cosj*sinii*radius);
				glVertex3f(sinjj*sinii*radius, cosii*radius, cosjj*sinii*radius);

				glVertex3f(sinj*sini*radius, cosi*radius, cosj*sini*radius);
				glVertex3f(sinjj*sinii*radius, cosii*radius, cosjj*sinii*radius);
				glVertex3f(sinjj*sini*radius, cosi*radius, cosjj*sini*radius);
				
				g += 0.04f;
			}
		}
	}

	glEnd();

	/*glBegin(GL_TRIANGLES);

	//Front Face
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f);
	glVertex3f(-lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f);
	glVertex3f(lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f);
	glVertex3f(lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f);
	glVertex3f(-lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f);
	glVertex3f(-lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f);

	//Left Face
	glColor3f(1.f, 1.f, 0.f);
	glVertex3f(-lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f);
	glVertex3f(-lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f);
	glVertex3f(-lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f);
	glVertex3f(-lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f);
	glVertex3f(-lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f);
	glVertex3f(-lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f);

	//Right Face
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f);
	glVertex3f(lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f);
	glVertex3f(lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f);
	glVertex3f(lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f);
	glVertex3f(lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f);
	glVertex3f(lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f);

	//Top Face
	glColor3f(0.f, 1.f, 1.f);
	glVertex3f(lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f);
	glVertex3f(-lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f);
	glVertex3f(lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f);
	glVertex3f(lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f);
	glVertex3f(-lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f);
	glVertex3f(-lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f);

	//Bottom Face
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f);
	glVertex3f(-lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f);
	glVertex3f(lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f);
	glVertex3f(lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f);
	glVertex3f(-lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f);
	glVertex3f(-lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f);

	//Back Face
	glColor3f(1.f, 0.f, 1.f);
	glVertex3f(-lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f);
	glVertex3f(lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f);
	glVertex3f(-lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f);
	glVertex3f(-lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f);
	glVertex3f(lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f);
	glVertex3f(lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f);

	glEnd();*/
}