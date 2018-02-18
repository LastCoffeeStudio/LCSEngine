#ifndef _MODEL_H_
#define _MODEL_H_

#include <map>

typedef unsigned int GLuint;
class aiScene;
class AssetTexture;
struct aiMesh;

class Model
{
public:
	Model();
	~Model();

	void Load(const char* file);
	void Clear();
	void Draw();

public:
	const aiScene* scene = 0;
	std::map<unsigned int, AssetTexture*> textures;
	
private:
	void DrawMesh(unsigned int meshNum);
};

#endif /*_MODEL_H_ */