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
#include "MaterialComponent.h"
#include "AnimationComponent.h"
#include <string.h>

SceneManager::SceneManager()
{
	filesPath.reserve(0);
}

SceneManager::~SceneManager() {}

void SceneManager::load(const char* file)
{
	scene = aiImportFile(file, aiProcessPreset_TargetRealtime_MaxQuality);
	int meshNum = 0;

	GameObject* root = createObject(App->sceneMain->root, scene->mRootNode);

	if (scene->HasAnimations())
	{
		ComponentFactory* factory = ComponentFactory::getInstance();
		AnimationComponent* anim = (AnimationComponent*)(factory->getComponent(ANIMATION, root));
		anim->rootBone = root;
		root->addComponent(anim);
	}
}

GameObject* SceneManager::createObject(GameObject* parent, aiNode* node) 
{
	GameObject* gameObject = new GameObject(parent, node->mName.C_Str());

	if (node->mNumMeshes > 0)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; ++i)
		{
			GameObject* gameObjectMesh = new GameObject(gameObject, node->mName.C_Str());

			unsigned int meshNum = node->mMeshes[i];

			ComponentFactory* factory = ComponentFactory::getInstance();
			MeshComponent* mesh = (MeshComponent*)(factory->getComponent(MESH, gameObjectMesh));

			aiMesh* currentMesh = scene->mMeshes[meshNum];
			mesh->verticesVBO = vector<float3>(currentMesh->mNumVertices, float3(0.f, 0.f, 0.f));
			mesh->normalsVBO = vector<float3>(currentMesh->mNumVertices, float3(0.f,0.f,0.f));
			mesh->texCoordsVBO = vector<float2>(currentMesh->mNumVertices, float2(0.f, 0.f));

			for (unsigned int l = 0; l < currentMesh->mNumVertices; ++l)
			{
				mesh->verticesVBO[l] = float3(currentMesh->mVertices[l].x,
					currentMesh->mVertices[l].y,
					currentMesh->mVertices[l].z);

				mesh->normalsVBO[l] = float3(currentMesh->mNormals[l].x,
					currentMesh->mNormals[l].y,
					currentMesh->mNormals[l].z);
				if (currentMesh->HasTextureCoords(0))
				{
					mesh->texCoordsVBO[l] = float2(currentMesh->mTextureCoords[0][l].x, currentMesh->mTextureCoords[0][l].y);
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
			gameObjectMesh->addComponent(mesh);

			//Create texture
			MaterialComponent* material = (MaterialComponent*)(factory->getComponent(MATERIAL, gameObjectMesh));
			aiMaterial* currentMaterial = scene->mMaterials[currentMesh->mMaterialIndex];
			aiString path;
			currentMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path);
			material->textureChanged = true;
			string texturePathName = path.C_Str();
			material->setNameTexture("Assets/Models/ArmyPilot/"+ texturePathName);
			gameObjectMesh->addComponent(material);

			gameObject->addGameObject(gameObjectMesh);
		}
	}

	((TransformComponent*)gameObject->components[0])->setTransform(node->mTransformation);

	parent->addGameObject(gameObject);
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		createObject(gameObject, node->mChildren[i]);
	}

	return gameObject;
}
