#include "Globals.h"
#include "SceneManager.h"
#include <postprocess.h>
#include <cimport.h>
#include <SDL_assert.h>

#include "Glew/include/glew.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleSceneMain.h"
#include "ComponentFactory.h"
#include "MeshComponent.h"
#include "TransformComponent.h"

SceneManager::SceneManager()
{
	filesPath.reserve(0);
}


SceneManager::~SceneManager()
{
}


void SceneManager::load(const char* file)
{
	scene = aiImportFile(file, aiProcessPreset_TargetRealtime_MaxQuality);
	int meshNum = 0;

	createObject(App->sceneMain->root, scene->mRootNode);
}

void SceneManager::createObject(GameObject* parent, aiNode* node) 
{
	GameObject* gameObject = new GameObject(parent, node->mName.C_Str());
	
	if (node->mNumMeshes > 0)
	{
		unsigned int meshNum = node->mMeshes[0];

		ComponentFactory* factory = ComponentFactory::getInstance();
		MeshComponent* mesh = (MeshComponent*)(factory->getComponent(MESH, parent));


		aiMesh* currentMesh = scene->mMeshes[meshNum];
        mesh->verticesVBO.clear();
		for (unsigned int l = 0; l < currentMesh->mNumVertices; ++l)
		{
			mesh->verticesVBO.push_back(float3(currentMesh->mVertices[l].x,
				currentMesh->mVertices[l].y,
				currentMesh->mVertices[l].z));

			mesh->normalsVBO.push_back(float3(currentMesh->mNormals[l].x,
				currentMesh->mNormals[l].y,
				currentMesh->mNormals[l].z));
			if (currentMesh->HasTextureCoords(meshNum))
			{
				mesh->texCoordsVBO.push_back(float2(currentMesh->mTextureCoords[0][l].x, currentMesh->mTextureCoords[0][l].y));
			}
		}

		for (unsigned k = 0; k < currentMesh->mNumFaces; ++k)
		{
			for (unsigned j = 0; j < currentMesh->mFaces[k].mNumIndices; ++j)
			{
				unsigned int index = currentMesh->mFaces[k].mIndices[j];
				mesh->indicesVAO.push_back(index);
			}
		}
        
        mesh->generateIDs();
		gameObject->addComponent(mesh);

		transformAiScene4x4ToFloat4x4(node->mTransformation, ((TransformComponent*)gameObject->components[0])->transform);
	}

	parent->addGameObject(gameObject);
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		createObject(gameObject, node->mChildren[i]);
	}
}

void SceneManager::transformAiScene4x4ToFloat4x4(const aiMatrix4x4 & matAiScene, float4x4 & matDest)
{
    //Copy and Transpose.
    matDest[0][0] = matAiScene[0][0];
    matDest[1][0] = matAiScene[0][1];
    matDest[2][0] = matAiScene[0][2];
    matDest[3][0] = matAiScene[0][3];
    matDest[0][1] = matAiScene[1][0];
    matDest[1][1] = matAiScene[1][1];
    matDest[2][1] = matAiScene[1][2];
    matDest[3][1] = matAiScene[1][3];
    matDest[0][2] = matAiScene[2][0];
    matDest[1][2] = matAiScene[2][1];
    matDest[2][2] = matAiScene[2][2];
    matDest[3][2] = matAiScene[2][3];
    matDest[0][3] = matAiScene[3][0];
    matDest[1][3] = matAiScene[3][1];
    matDest[2][3] = matAiScene[3][2];
    matDest[3][3] = matAiScene[3][3];
}

