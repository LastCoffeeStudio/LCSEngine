#ifndef __MESHCOMPONENT_H__
#define __MESHCOMPONENT_H__

#include "Component.h"
#include "MathGeoLib/src/Math/float2.h"
#include "MathGeoLib/src/Math/float3.h"
#include <vector>
#include <glew.h>

enum PresetType
{
	TRIANGLE = 1,
	CUBE,
	SPHERE,
	MODEL
};

class Model;

class MeshComponent : public Component
{
public:
	MeshComponent(GameObject* gameObject, bool isEnable = true, bool isUnique = true);
	~MeshComponent();

	void drawGUI() override;
	bool update() override;
	std::vector<float3> verticesVBO;
	std::vector<float3> colorsVBO;
	std::vector<float3> normalsVBO;
	std::vector<float2> texCoordsVBO;
	std::vector<unsigned int> indicesVAO;
	GLuint idVertVBO = 0;
	GLuint idNormVBO = 0;
	GLuint idIdxVAO = 0;
	GLuint idTexCoords = 0;
	GLuint idColors = 0;
	//TODO: save textureID from model
	float lengthX, lengthY, lengthZ;
	void setPreset(PresetType type);
	void updateColor(const float3& color);
    void generateIDs();
    void loadPreset();
public:
	PresetType currentPreset = CUBE;
	Model* model = nullptr;

private:
	void loadModel();
	void loadSphere();
	void loadCube();
};

#endif //__MESHCOMPONENT_H__