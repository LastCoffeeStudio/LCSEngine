#include "Application.h"
#include "ModuleSceneMain.h"
#include "ModuleTextures.h"
#include "MeshComponent.h"
#include "Imgui/imgui.h"
#include "Model.h"
#include "AssetTexture.h"
#include "assimp/include/scene.h"

static int selected_preset = 1;
const char* presets[] = { "House", "Cube", "Sphere" };

MeshComponent::MeshComponent(GameObject* gameObject, bool isEnable, bool isUnique) : Component(gameObject, isEnable, isUnique)
{
	typeComponent = MESH;
	loadPreset();
}

MeshComponent::~MeshComponent() {}

bool MeshComponent::update()
{
	return true;
}

void MeshComponent::setPreset(PresetType type)
{
	currentPreset = type;
	loadPreset();
}

void MeshComponent::loadPreset()
{
	verticesVBO.clear();
	currentPreset = MODEL;
	switch (currentPreset)
	{
		case CUBE:
		{
			loadCube();
			break;
		}
		case MODEL:
		{
			if (!model)
			{
				model = new Model();
				model->Load("Assets/Models/BakerHouse.fbx");
			}
			loadModel();
			break;
		}
		case SPHERE:
		{
			loadSphere();
		}
	}

    generateIDs();
}
void MeshComponent::generateIDs()
{
    //Clean buffers
    glDeleteBuffers(1, (GLuint*) &(idVertVBO));
    glDeleteBuffers(1, (GLuint*) &(idIdxVAO));
    glDeleteBuffers(1, (GLuint*) &(idTexCoords));
    glDeleteBuffers(1, (GLuint*) &(idColors));
    //glDeleteBuffers(1, (GLuint*) &(idNormVBO));

    glGenBuffers(1, (GLuint*) &(idVertVBO));
    glBindBuffer(GL_ARRAY_BUFFER, idVertVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesVBO.size() * 3, verticesVBO[0].ptr(), GL_STATIC_DRAW);

    glGenBuffers(1, (GLuint*) &(idIdxVAO));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIdxVAO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesVAO.size(), &indicesVAO[0], GL_STATIC_DRAW);

    if (texCoordsVBO.size() > 0)
    {
        glGenBuffers(1, (GLuint*) &(idTexCoords));
        glBindBuffer(GL_ARRAY_BUFFER, idTexCoords);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texCoordsVBO.size() * 2, texCoordsVBO[0].ptr(), GL_STATIC_DRAW);
    }
    colorsVBO.clear();
    for (unsigned int i = 0; i < verticesVBO.size(); ++i)
    {
        colorsVBO.push_back(float3(1.f, 1.f, 1.f));
    }

    glGenBuffers(1, (GLuint*) &(idColors));
    glBindBuffer(GL_ARRAY_BUFFER, idColors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colorsVBO.size() * 3, colorsVBO[0].ptr(), GL_STATIC_DRAW);

    /*glGenBuffers(1, (GLuint*) &(idNormVBO));
    glBindBuffer(GL_ARRAY_BUFFER, idNormVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normalsVBO.size() * 3, normalsVBO[0].ptr(), GL_STATIC_DRAW);*/
}

void MeshComponent::drawGUI()
{
	if (ImGui::CollapsingHeader("Mesh"))
	{
		ImGui::Checkbox("Active", &isEnable); ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(0.6f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(0.6f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(0.6f, 0.8f, 0.8f));
		if (ImGui::Button("Delete Component"))
		{
			App->sceneMain->garbageCollectorComponent.push_back(this);
		}
		ImGui::TextColored({0.8f, 0.8f, 0.3f, 1.f}, selected_preset == -1 ? "<None>" : presets[selected_preset]);
		if (ImGui::Button("Change Preset.."))
		{
			ImGui::OpenPopup("select preset");
		}
		if (ImGui::BeginPopup("select preset"))
		{
			for (int i = 0; i < IM_ARRAYSIZE(presets); i++)
			{
				if (ImGui::Selectable(presets[i]))
				{
					selected_preset = i;
					//Set Mesh of preset selected
					if (i == 0)
					{
						currentPreset = PresetType::MODEL;
					}
					else if (i == 1)
					{
						currentPreset = PresetType::CUBE;
					}
					else if (i == 2)
					{
						currentPreset = PresetType::SPHERE;
					}
					loadPreset();
				}
			}
			ImGui::EndPopup();
		}
		ImGui::PopStyleColor(3);
		ImGui::TextColored({ 0.8f, 0.8f, 0.3f, 1.f }, "12 triangles (36 indices, 16 Vertices)");
	}
}

void MeshComponent::loadSphere()
{
	float radius = 1.f;
	int nSeg = 16;

	int nRowsQuads = (nSeg - 4) / 2;		//nº rows with quads on the sphere
	int vertRQ = nSeg * 18;					//2 triangles/quad * 3 vertices/triangle * 3 coords/vertex
	int vertSide = nSeg * 9;				//3 vertices/triangle * 3 coords/vertex
	unsigned int totalCoordsVBO = vertSide * 2 + nRowsQuads * vertRQ;
	verticesVBO.reserve(totalCoordsVBO);

	float difa = 360.f / (float)nSeg;
	float r, g, b;
	r = g = b = 0.2f;

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
				verticesVBO.push_back(float3(sinj*sini*radius, cosi*radius, cosj*sini*radius));
				verticesVBO.push_back(float3(sinj*sinii*radius, cosii*radius, cosj*sinii*radius));
				verticesVBO.push_back(float3(sinjj*sinii*radius, cosii*radius, cosjj*sinii*radius));

				g += 0.001f;
			}
			else if (i == (nSeg / 2) - 1)
			{
				g = b = 0.f;

				verticesVBO.push_back(float3(sinj*sini*radius, cosi*radius, cosj*sini*radius));
				verticesVBO.push_back(float3(sinj*sinii*radius, cosii*radius, cosj*sinii*radius));
				verticesVBO.push_back(float3(sinjj*sini*radius, cosi*radius, cosjj*sini*radius));

				r += 0.001f;
			}
			else
			{
				g = 0.f;
				verticesVBO.push_back(float3(sinj*sini*radius, cosi*radius, cosj*sini*radius));
				verticesVBO.push_back(float3(sinj*sinii*radius, cosii*radius, cosj*sinii*radius));
				verticesVBO.push_back(float3(sinjj*sinii*radius, cosii*radius, cosjj*sinii*radius));


				verticesVBO.push_back(float3(sinj*sini*radius, cosi*radius, cosj*sini*radius));
				verticesVBO.push_back(float3(sinjj*sinii*radius, cosii*radius, cosjj*sinii*radius));
				verticesVBO.push_back(float3(sinjj*sini*radius, cosi*radius, cosjj*sini*radius));

				b += 0.001f;
			}
		}
	}
}

void MeshComponent::loadCube()
{
	verticesVBO.reserve(8);
	indicesVAO.reserve(36);
	lengthX = lengthY = lengthZ = 1.f;

	verticesVBO = { float3(-lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f), float3(lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f),
		float3(lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f), float3(-lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f),
		float3(-lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f), float3(lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f),
		float3(lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f), float3(-lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f) };

	indicesVAO = { 1, 3, 2, 1, 0, 3,
		0, 7, 3, 0, 4, 7,
		5, 2, 6, 5, 1, 2,
		5, 0, 1, 5, 4, 0,
		2, 7, 6, 2, 3, 7,
		4, 6, 7, 4, 5, 6 };
}

void MeshComponent::loadModel()
{
	for (unsigned i = 0; i < model->scene->mRootNode->mNumChildren; ++i)
	{
		int verticesVBOsize = verticesVBO.size();
		unsigned int meshNum = model->scene->mRootNode->mChildren[i]->mMeshes[0];
		aiMesh* currentMesh = model->scene->mMeshes[meshNum];

		for (unsigned int l = 0; l < currentMesh->mNumVertices; ++l)
		{
			verticesVBO.push_back(float3(currentMesh->mVertices[l].x,
				currentMesh->mVertices[l].y,
				currentMesh->mVertices[l].z));

			normalsVBO.push_back(float3(currentMesh->mNormals[l].x,
				currentMesh->mNormals[l].y,
				currentMesh->mNormals[l].z));

			texCoordsVBO.push_back(float2(currentMesh->mTextureCoords[0][l].x, currentMesh->mTextureCoords[0][l].y));
		}

		for (unsigned k = 0; k < currentMesh->mNumFaces; ++k)
		{
			for (unsigned j = 0; j < currentMesh->mFaces[k].mNumIndices; ++j)
			{
				unsigned int index = currentMesh->mFaces[k].mIndices[j];
				indicesVAO.push_back(index + verticesVBOsize);
			}
		}
	}
}

void MeshComponent::updateColor(const float3& color)
{
	for (unsigned int i = 0; i < colorsVBO.size(); ++i)
	{
		colorsVBO[i] = float3(color.x,color.y,color.z);
	}

	glBindBuffer(GL_ARRAY_BUFFER, idColors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colorsVBO.size() * 3, colorsVBO[0].ptr(), GL_STATIC_DRAW);
}