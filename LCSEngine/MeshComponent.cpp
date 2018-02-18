#include "MeshComponent.h"
#include "Imgui/imgui.h"

static int selected_preset = 1;
const char* presets[] = { "Triangle", "Cube", "Sphere" };

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
	switch (currentPreset)
	{
		case CUBE:
		{
			loadCube();
			break;
		}
		case TRIANGLE:
		{

			break;
		}
		case SPHERE:
		{
			loadSphere();
		}
	}

	glGenBuffers(1, (GLuint*) &(idVertVBO));
	glBindBuffer(GL_ARRAY_BUFFER, idVertVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesVBO.size() * 3, verticesVBO[0].ptr(), GL_STATIC_DRAW);
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
			suicide = true;
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
						currentPreset = PresetType::TRIANGLE;
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
	verticesVBO.reserve(36);
	lengthX = lengthY = lengthZ = 1.f;

	verticesVBO = { float3(lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f), float3(-lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f), float3(lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f),
		float3(lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f), float3(-lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f), float3(-lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f),
		float3(-lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f), float3(-lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f), float3(-lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f),
		float3(-lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f), float3(-lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f), float3(-lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f),
		float3(lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f), float3(lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f), float3(lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f),
		float3(lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f), float3(lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f), float3(lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f),
		float3(lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f), float3(-lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f), float3(lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f),
		float3(lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f), float3(-lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f), float3(-lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f),
		float3(lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f), float3(-lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f), float3(lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f),
		float3(lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f), float3(-lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f), float3(-lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f),
		float3(-lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f), float3(lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f), float3(-lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f),
		float3(-lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f), float3(lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f), float3(lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f) };
}

