#include "Primitive.h"

Primitive::Primitive()
{
	renderMode = DIRECTMODE;
}

Primitive::~Primitive() {}

void Primitive::draw()
{
	switch (renderMode)
	{
	case DIRECTMODE:
		drawDirectMode();
		break;

	case VBO:
		drawVBO();
		break;

	case VERTEXARRAY:
		drawVertexArray();
		break;
	}
}

void Primitive::changeRenderMode(bool left)
{
	switch (renderMode)
	{
	case DIRECTMODE:
		(left == true ? renderMode = VERTEXARRAY : renderMode = VBO);
		glClearColor(0.f, 0.f, 0.f, 1.f);
		break;

	case VBO:
		(left == true ? renderMode = DIRECTMODE : renderMode = VERTEXARRAY);
		glClearColor(0.f, 0.f, 0.f, 1.f);
		break;

	case VERTEXARRAY:
		(left == true ? renderMode = VBO : renderMode = DIRECTMODE);
		glClearColor(0.f, 0.f, 0.f, 1.f);
		break;
	}
}

void Primitive::initializeValues()
{
	idVertVBO = idColVBO = idVertVA = idIndVA = idColVA = 0;
}

bool Primitive::cleanUp()
{
	return true;
}

void Primitive::drawDirectMode() {}

void Primitive::drawVBO()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, idVertVBO);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, idColVBO);
	glColorPointer(3, GL_FLOAT, 0, NULL);
	glDrawArrays(GL_TRIANGLES, 0, verticesVBO.size());

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void Primitive::drawVertexArray()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, idVertVA);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, idColVA);
	glColorPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndVA);

	glDrawElements(GL_TRIANGLES, indicesVA.size(), GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}