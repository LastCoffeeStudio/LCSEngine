#include "Application.h"
#include "Model.h"
#include "ModuleTextures.h"
#include "AssetTexture.h"


#include "assimp/include/scene.h"
#include "assimp/include/cimport.h"
#include "assimp/include/postprocess.h" 
#include "Glew/include/glew.h"

Model::Model() {}

Model::~Model() {}

void Model::Load(const char* file)
{
	scene = aiImportFile(file, aiProcessPreset_TargetRealtime_MaxQuality);
	//App->textures->loadModelTextures(scene);
}

void Model::Clear()
{
	aiReleaseImport(scene);
}

void Model::Draw() {
	for (unsigned i = 0; i < scene->mRootNode->mNumChildren; ++i)
	{
		DrawMesh(scene->mRootNode->mChildren[i]->mMeshes[0]);
	}
}

void Model::DrawMesh(unsigned int meshNum)
{	
	for (unsigned i = 0; i < scene->mMeshes[meshNum]->mNumFaces; ++i)
	{
		glBegin(GL_TRIANGLES);
		for (unsigned j = 0; j < scene->mMeshes[meshNum]->mFaces[i].mNumIndices; ++j)
		{
			int index = scene->mMeshes[meshNum]->mFaces[i].mIndices[j];
			
			if (scene->mMeshes[meshNum]->HasTextureCoords(0))
			{
				glTexCoord2f(scene->mMeshes[meshNum]->mTextureCoords[0][index].x, 1 - scene->mMeshes[meshNum]->mTextureCoords[0][index].y);
			}
			
			glNormal3fv(&scene->mMeshes[meshNum]->mNormals[index].x);
			glVertex3fv(&scene->mMeshes[meshNum]->mVertices[index].x);
		}
		glEnd();
	}
}

