#ifndef __BILLBOARDGRIDCOMPONENT_H__
#define __BILLBOARDGRIDCOMPONENT_H__

#include "Component.h"
#include "MathGeoLib/src/Math/float2.h"
#include "MathGeoLib/src/Math/float3.h"
#include "MathGeoLib/src/Math/float4x4.h"

typedef unsigned int GLuint;
class Billboard;

class BillboardGridComponent : public Component
{
public:
	BillboardGridComponent(GameObject* gameObject, bool isEnable = true, bool isUnique = false);
	~BillboardGridComponent();

	void calculateVertexs();
	void drawGUI() override;

public:
	std::vector<Billboard*> billboards;
	std::vector<float3> verticesVBO;
	std::vector<float3> colorsVBO;
	std::vector<float2> texCoordsVBO;
	std::vector<unsigned int> indicesVBO;
	GLuint idVertVBO = 0;
	GLuint idIdxVAO = 0;
	GLuint idTexCoords = 0;
	GLuint idColors = 0;
	GLuint texID = 0;
	bool hasTexture = false;
	int n = 5;
	int m = 8;
	float minW = 0.9f;
	float maxW = 1.1f;
	float minH = 0.8f;
	float maxH = 1.2f;
	float quadSpaceX = 0.5f;
	float quadSpaceY = 0.5f;

private:
	void updateBillboards();

};

#endif //__BILLBOARDGRIDCOMPONENT_H__