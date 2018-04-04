#ifndef __MESHCOMPONENT_H__
#define __MESHCOMPONENT_H__

#include "Component.h"
#include "MathGeoLib/src/Math/float2.h"
#include "MathGeoLib/src/Math/float3.h"
#include "MathGeoLib/src/Math/float4x4.h"
#include <vector>
#include <glew.h>

enum PresetType
{
	TRIANGLE = 1,
	CUBE,
	SPHERE,
	MODEL
};

struct Weight
{
	unsigned int vertex = 0;
	float weight = 0.f;
};

struct Bone
{
	std::string name = "";
	std::vector<Weight*> weights;
	float4x4 bind;
};

class Model;

class MeshComponent : public Component
{
public:
	MeshComponent(GameObject* gameObject, bool isEnable = true, bool isUnique = true);
	~MeshComponent();

	void drawGUI() override;
	bool update() override;
	void setPreset(PresetType type);
	void updateColor(const float3& color);
	void generateIDs();
	void loadPreset();
	void updateVerticesBuffer();
	void load(nlohmann::json& conf) override;
	void save(nlohmann::json& conf) override;

public:
	std::vector<float3> verticesVBO;
	std::vector<float3> colorsVBO;
	std::vector<float3> normalsVBO;
	std::vector<float2> texCoordsVBO;
	std::vector<unsigned int> indicesVAO;
	std::vector<Bone*> bones;
	std::vector<float3> originalVertices;
	GLuint idVertVBO = 0;
	GLuint idNormVBO = 0;
	GLuint idIdxVAO = 0;
	GLuint idTexCoords = 0;
	GLuint idColors = 0;
	//TODO: save textureID from model
	float lengthX, lengthY, lengthZ;
	PresetType currentPreset = CUBE;
	Model* model = nullptr;
	bool validMesh = false;

private:
	void loadModel();
	void loadSphere();
	void loadCube();
};

#endif //__MESHCOMPONENT_H__