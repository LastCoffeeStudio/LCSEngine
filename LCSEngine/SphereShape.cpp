#include "Globals.h"
#include "Application.h"
#include "SphereShape.h"
#include "ModuleTextures.h"

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
	totalTexCoordsVBO = nSeg * 12 + ((nSeg - 4) / 2) * nSeg * 12;

	verticesVBO.reserve(totalCoordsVBO);
	colorsVBO.reserve(totalCoordsVBO);
	texcoordsVBO.reserve(totalTexCoordsVBO);
	verticesVA.reserve((nRowsQuads + 1) * nSeg + 2);
	indicesVA.reserve(nRowsQuads*nSeg * 2 * 3 + nSeg * 3 * 2);
	colorsVA.reserve((nRowsQuads + 1) * nSeg + 2);

	float difa = 360.f / (float)nSeg;
	float r, g, b;
	r = g = b = 0.2f;
	int idNextV = 0;
	int firstV, secondV, thirdV, fourthV;
	firstV = secondV = thirdV = fourthV = 0;
	float texCoordXDif = 1.f / (float)nSeg;
	float texCoordYDif = 1.f / ((float)nSeg / 2.f);

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

			//Adjust last vertex if nSeg odd
			if (i == (nSeg / 2) - 1 && nSeg % 2 != 0)
			{
				sinii = 0;
				cosii = -1;
			}

			if (i == 0)
			{
				colorsVBO.push_back(r);
				colorsVBO.push_back(g);
				colorsVBO.push_back(b);
				colorsVBO.push_back(r);
				colorsVBO.push_back(g);
				colorsVBO.push_back(b);
				colorsVBO.push_back(r);
				colorsVBO.push_back(g);
				colorsVBO.push_back(b);
				verticesVBO.push_back(sinj*sini*radius);
				verticesVBO.push_back(cosi*radius);
				verticesVBO.push_back(cosj*sini*radius);
				verticesVBO.push_back(sinj*sinii*radius);
				verticesVBO.push_back(cosii*radius);
				verticesVBO.push_back(cosj*sinii*radius);
				verticesVBO.push_back(sinjj*sinii*radius);
				verticesVBO.push_back(cosii*radius);
				verticesVBO.push_back(cosjj*sinii*radius);
				texcoordsVBO.push_back(j*texCoordXDif);
				texcoordsVBO.push_back(i*texCoordYDif);
				texcoordsVBO.push_back(j*texCoordXDif);
				texcoordsVBO.push_back((i+1)*texCoordYDif);
				texcoordsVBO.push_back((j+1)*texCoordXDif);
				texcoordsVBO.push_back((i+1)*texCoordYDif);

				if (j == 0)
				{
					colorsVA.push_back(r);
					colorsVA.push_back(g);
					colorsVA.push_back(b);
					verticesVA.push_back(sinj*sini*radius);
					verticesVA.push_back(cosi*radius);
					verticesVA.push_back(cosj*sini*radius);
					firstV = idNextV;
					secondV = idNextV + 1;
					thirdV = idNextV + 1;
					idNextV += 2;
				}
				colorsVA.push_back(r);
				colorsVA.push_back(g);
				colorsVA.push_back(b);
				verticesVA.push_back(sinj*sinii*radius);
				verticesVA.push_back(cosii*radius);
				verticesVA.push_back(cosj*sinii*radius);

				indicesVA.push_back(firstV);
				indicesVA.push_back(thirdV);
				if (j < nSeg - 1)
				{
					indicesVA.push_back(idNextV);
					thirdV = idNextV;
					++idNextV;
				}
				else
				{
					indicesVA.push_back(secondV);
				}

				g += 0.001f;
			}
			else if (i == (nSeg / 2) - 1)
			{
				g = b = 0.f;
				colorsVBO.push_back(r);
				colorsVBO.push_back(g);
				colorsVBO.push_back(b);
				colorsVBO.push_back(r);
				colorsVBO.push_back(g);
				colorsVBO.push_back(b);
				colorsVBO.push_back(r);
				colorsVBO.push_back(g);
				colorsVBO.push_back(b);
				verticesVBO.push_back(sinj*sini*radius);
				verticesVBO.push_back(cosi*radius);
				verticesVBO.push_back(cosj*sini*radius);
				verticesVBO.push_back(sinj*sinii*radius);
				verticesVBO.push_back(cosii*radius);
				verticesVBO.push_back(cosj*sinii*radius);
				verticesVBO.push_back(sinjj*sini*radius);
				verticesVBO.push_back(cosi*radius);
				verticesVBO.push_back(cosjj*sini*radius);
				texcoordsVBO.push_back(j*texCoordXDif);
				texcoordsVBO.push_back(i*texCoordYDif);
				texcoordsVBO.push_back(j*texCoordXDif);
				texcoordsVBO.push_back((i+1)*texCoordYDif);
				texcoordsVBO.push_back((j+1)*texCoordXDif);
				texcoordsVBO.push_back(i*texCoordYDif);

				if (j == 0)
				{
					firstV = idNextV - nSeg;
					secondV = idNextV;
					thirdV = idNextV - nSeg;
					verticesVA.push_back(sinj*sinii*radius);
					verticesVA.push_back(cosii*radius);
					verticesVA.push_back(cosj*sinii*radius);
				}

				if (j < nSeg - 1)
				{
					indicesVA.push_back(thirdV);
					indicesVA.push_back(secondV);
					indicesVA.push_back(thirdV + 1);
					++thirdV;
				}
				else
				{
					indicesVA.push_back(thirdV);
					indicesVA.push_back(secondV);
					indicesVA.push_back(firstV);
				}

				r += 0.001f;
			}
			else
			{
				g = 0.f;
				colorsVBO.push_back(r);
				colorsVBO.push_back(g);
				colorsVBO.push_back(b);
				colorsVBO.push_back(r);
				colorsVBO.push_back(g);
				colorsVBO.push_back(b);
				colorsVBO.push_back(r);
				colorsVBO.push_back(g);
				colorsVBO.push_back(b);
				verticesVBO.push_back(sinj*sini*radius);
				verticesVBO.push_back(cosi*radius);
				verticesVBO.push_back(cosj*sini*radius);
				verticesVBO.push_back(sinj*sinii*radius);
				verticesVBO.push_back(cosii*radius);
				verticesVBO.push_back(cosj*sinii*radius);
				verticesVBO.push_back(sinjj*sinii*radius);
				verticesVBO.push_back(cosii*radius);
				verticesVBO.push_back(cosjj*sinii*radius);
				texcoordsVBO.push_back(j*texCoordXDif);
				texcoordsVBO.push_back(i*texCoordYDif);
				texcoordsVBO.push_back(j*texCoordXDif);
				texcoordsVBO.push_back((i+1)*texCoordYDif);
				texcoordsVBO.push_back((j+1)*texCoordXDif);
				texcoordsVBO.push_back((i+1)*texCoordYDif);

				colorsVBO.push_back(r);
				colorsVBO.push_back(g);
				colorsVBO.push_back(b);
				colorsVBO.push_back(r);
				colorsVBO.push_back(g);
				colorsVBO.push_back(b);
				colorsVBO.push_back(r);
				colorsVBO.push_back(g);
				colorsVBO.push_back(b);
				verticesVBO.push_back(sinj*sini*radius);
				verticesVBO.push_back(cosi*radius);
				verticesVBO.push_back(cosj*sini*radius);
				verticesVBO.push_back(sinjj*sinii*radius);
				verticesVBO.push_back(cosii*radius);
				verticesVBO.push_back(cosjj*sinii*radius);
				verticesVBO.push_back(sinjj*sini*radius);
				verticesVBO.push_back(cosi*radius);
				verticesVBO.push_back(cosjj*sini*radius);
				texcoordsVBO.push_back(j*texCoordXDif);
				texcoordsVBO.push_back(i*texCoordYDif);
				texcoordsVBO.push_back((j+1)*texCoordXDif);
				texcoordsVBO.push_back((i+1)*texCoordYDif);
				texcoordsVBO.push_back((j + 1)*texCoordXDif);
				texcoordsVBO.push_back(i*texCoordYDif);

				if (j == 0)
				{
					firstV = idNextV - nSeg;
					secondV = idNextV;
					thirdV = idNextV - nSeg;
					fourthV = idNextV;
				}

				colorsVA.push_back(r);
				colorsVA.push_back(g);
				colorsVA.push_back(b);
				verticesVA.push_back(sinj*sinii*radius);
				verticesVA.push_back(cosii*radius);
				verticesVA.push_back(cosj*sinii*radius);

				if (j < nSeg - 1)
				{
					indicesVA.push_back(thirdV);
					indicesVA.push_back(fourthV);
					indicesVA.push_back(fourthV + 1);
					indicesVA.push_back(thirdV);
					indicesVA.push_back(fourthV + 1);
					indicesVA.push_back(thirdV + 1);
					++thirdV;
					++fourthV;
				}
				else
				{
					indicesVA.push_back(thirdV);
					indicesVA.push_back(fourthV);
					indicesVA.push_back(secondV);
					indicesVA.push_back(thirdV);
					indicesVA.push_back(secondV);
					indicesVA.push_back(firstV);
				}
				++idNextV;

				b += 0.001f;
			}
		}
	}

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

bool SphereShape::cleanUp()
{
	glDeleteBuffers(1, &(idVertVBO));
	glDeleteBuffers(1, &(idColVBO));
	glDeleteBuffers(1, &(idVertVA));
	glDeleteBuffers(1, &(idIndVA));
	glDeleteBuffers(1, &(idColVA));

	return true;
}

void SphereShape::drawDirectMode()
{
	/** We fix a pivot on the center of the cube, i.e. we divide by 2 **/

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, App->textures->ImageName);
	glBegin(GL_TRIANGLES);

	float difa = 360.f / (float)nSeg;
	float r, g, b;
	r = g = b = 0.2f;
	float texCoordXDif = 1.f / (float)nSeg;
	float texCoordYDif = 1.f / ((float)nSeg / 2.f);


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

			//Adjust last vertex if nSeg odd
			if (i == (nSeg / 2) - 1 && nSeg % 2 != 0)
			{
				sinii = 0;
				cosii = -1;
			}

			if (i == 0)
			{
				glColor3f(r, g, b);
				glTexCoord2f(j*texCoordXDif, i*texCoordYDif);
				glVertex3f(sinj*sini*radius, cosi*radius, cosj*sini*radius);
				glTexCoord2f(j*texCoordXDif, (i + 1)*texCoordYDif);
				glVertex3f(sinj*sinii*radius, cosii*radius, cosj*sinii*radius);
				glTexCoord2f((j + 1)*texCoordXDif, (i + 1)*texCoordYDif);
				glVertex3f(sinjj*sinii*radius, cosii*radius, cosjj*sinii*radius);
				r += 0.001f;
			}
			else if (i == (nSeg / 2) - 1)
			{
				r = g = 0.f;
				glColor3f(r, g, b);
				glTexCoord2f((j + 1)*texCoordXDif, i*texCoordYDif);
				glVertex3f(sinjj*sini*radius, cosi*radius, cosjj*sini*radius);
				glTexCoord2f(j*texCoordXDif, i*texCoordYDif);
				glVertex3f(sinj*sini*radius, cosi*radius, cosj*sini*radius);
				glTexCoord2f(j*texCoordXDif, (i + 1)*texCoordYDif);
				glVertex3f(sinj*sinii*radius, cosii*radius, cosj*sinii*radius);
				b += 0.001f;
			}
			else
			{
				r = 0.f;
				glColor3f(r, g, b);
				glTexCoord2f(j*texCoordXDif, i*texCoordYDif);
				glVertex3f(sinj*sini*radius, cosi*radius, cosj*sini*radius);
				glTexCoord2f(j*texCoordXDif, (i + 1)*texCoordYDif);
				glVertex3f(sinj*sinii*radius, cosii*radius, cosj*sinii*radius);
				glTexCoord2f((j + 1)*texCoordXDif, (i + 1)*texCoordYDif);
				glVertex3f(sinjj*sinii*radius, cosii*radius, cosjj*sinii*radius);

				glTexCoord2f(j*texCoordXDif, i*texCoordYDif);
				glVertex3f(sinj*sini*radius, cosi*radius, cosj*sini*radius);
				glTexCoord2f((j + 1)*texCoordXDif, (i + 1)*texCoordYDif);
				glVertex3f(sinjj*sinii*radius, cosii*radius, cosjj*sinii*radius);
				glTexCoord2f((j + 1)*texCoordXDif, i*texCoordYDif);
				glVertex3f(sinjj*sini*radius, cosi*radius, cosjj*sini*radius);

				g += 0.001f;
			}
		}
	}

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
}