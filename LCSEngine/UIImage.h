#ifndef __UIIMAGE_H__
#define __UIIMAGE_H__

#include "ElementGameUI.h"
#include "MathGeoLib/src/Math/float2.h"
#include "MathGeoLib/src/Math/float3.h"

typedef unsigned int GLuint;

class UIImage : public ElementGameUI
{
public:
	UIImage(GameObject* parent, int x, int y, int h, int w, bool isVisible = true);
	UIImage(GameObject* parent);
	~UIImage();
	void drawGUI() override;
	void fillGUI();
	void updateCoords();
	void load(nlohmann::json& conf);
	void save(nlohmann::json& conf);

public:
	GLuint texID = 0;
	GLuint idVertVBO = 0;
	GLuint idIdxVAO = 0;
	GLuint idTexCoords = 0;
	GLuint idColors = 0;
	std::string texName = "";
	char textureName[128] = "";
	bool textureChanged = false;
	bool hasTexture = false;

private:
	std::vector<float3> verticesVBO;
	std::vector<float3> colorsVBO;
	std::vector<float2> texCoordsVBO;
	std::vector<unsigned int> indicesVAO;

private:
	void init();
	void generateIDs();
	void generateBuffers();

};

#endif //__UIIMAGE_H__