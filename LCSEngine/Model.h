#ifndef _MODEL_H_
#define _MODEL_H_

class aiScene;

class Model
{
	const aiScene* scene = 0;

public:
	Model();
	~Model();

	void Load(const char* file);
	void Clear();
	void Draw();
};

#endif /*_MODEL_H_ */