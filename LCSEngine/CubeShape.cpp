#include "Globals.h"
#include "Application.h"
#include "CubeShape.h"
#include "AssetTexture.h"
#include "ModuleSceneMain.h"

CubeShape::CubeShape()
{
	lengthX = lengthY = lengthZ = 1.f;
}

CubeShape::CubeShape(float lengthX, float lengthY, float lengthZ) : lengthX(lengthX), lengthY(lengthY), lengthZ(lengthZ) {}

CubeShape::~CubeShape() {}

void CubeShape::initializeValues()
{
	verticesVBO.reserve(108);	//3 coords/vertex * 6 vertices/face * 6 faces
	colorsVBO.reserve(108);		//3 values/color * 1 color/vertex * 36 vertices
	texcoordsVBO.reserve(72);
	verticesVA.reserve(24);		//3 coors/vertex * 8 vertices
	indicesVA.reserve(36);		//6 indices/face * 6 faces
	colorsVA.reserve(24);		//3 colors/vertex * 8 vertices

	verticesVBO = { lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f, -lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f, lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f,
		lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f, -lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f, -lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f,
		-lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f, -lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f, -lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f,
		-lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f, -lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f, -lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f,
		lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f, lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f, lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f,
		lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f, lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f, lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f,
		lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f, -lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f, lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f,
		lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f, -lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f, -lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f,
		lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f, -lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f, lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f,
		lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f, -lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f, -lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f,
		-lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f, lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f, -lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f,
		-lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f, lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f, lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f };

	texcoordsVBO = { 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f, 0.f, 0.f,
					 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f, 0.f, 0.f,
					 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f, 0.f, 0.f,
					 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f, 0.f, 0.f,
					 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f, 0.f, 0.f,
					 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f, 0.f, 0.f, };

	float r, g, b;
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
				  4, 6, 7, 4, 5, 6 };

	/** NOTE: If we want each face of one colour instead of assigning each vertex a colour, we must create 24 vertices
	(4 for each face, so the array would be of size 72) instead of 8. That way we can assign a single colour for 4 vertices
	of the same face, and the others as well **/
	colorsVA = { 1.f, 0.f, 0.f,
				 0.f, 1.f, 0.f,
				 0.f, 1.f, 1.f,
				 0.f, 0.f, 1.f,
				 0.f, 1.f, 1.f,
				 0.f, 0.f, 1.f,
				 1.f, 0.f, 0.f,
				 0.f, 1.f, 0.f };

	idVertVBO = 0;	//Assign initial value in case GenBuffers fails
	glGenBuffers(1, (GLuint*) &(idVertVBO));
	glBindBuffer(GL_ARRAY_BUFFER, idVertVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesVBO.size(), &verticesVBO[0], GL_STATIC_DRAW);

	idColVBO = 1;
	glGenBuffers(1, (GLuint*) &(idColVBO));
	glBindBuffer(GL_ARRAY_BUFFER, idColVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colorsVBO.size(), &colorsVBO[0], GL_STATIC_DRAW);

	idTexCoordVBO = 2;
	glGenBuffers(1, (GLuint*) &(idTexCoordVBO));
	glBindBuffer(GL_ARRAY_BUFFER, idTexCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texcoordsVBO.size(), &texcoordsVBO[0], GL_STATIC_DRAW);

	idVertVA = 3;
	glGenBuffers(1, (GLuint*) &(idVertVA));
	glBindBuffer(GL_ARRAY_BUFFER, idVertVA);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesVA.size(), &verticesVA[0], GL_STATIC_DRAW);

	idIndVA = 4;
	glGenBuffers(1, (GLuint*) &(idIndVA));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndVA);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesVA.size(), &indicesVA[0], GL_STATIC_DRAW);

	idColVA = 5;
	glGenBuffers(1, (GLuint*) &(idColVA));
	glBindBuffer(GL_ARRAY_BUFFER, idColVA);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colorsVA.size(), &colorsVA[0], GL_STATIC_DRAW);
}

bool CubeShape::cleanUp()
{
	glDeleteBuffers(1, &(idVertVBO));
	glDeleteBuffers(1, &(idColVBO));
	glDeleteBuffers(1, &(idVertVA));
	glDeleteBuffers(1, &(idIndVA));
	glDeleteBuffers(1, &(idColVA));

	return true;
}

void CubeShape::drawDirectMode()
{
	/** We fix a pivot on the center of the cube, i.e. we divide by 2 **/
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, App->sceneMain->actual->ID);
	glBegin(GL_TRIANGLES);

	//Front Face
	glColor3f(1.f, 0.f, 0.f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f);

	//Left Face
	glColor3f(1.f, 1.f, 0.f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f);

	//Right Face
	glColor3f(0.f, 1.f, 0.f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f);

	//Top Face
	glColor3f(0.f, 1.f, 1.f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f);

	//Bottom Face
	glColor3f(0.f, 0.f, 1.f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f);

	//Back Face
	glColor3f(1.f, 0.f, 1.f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);

}