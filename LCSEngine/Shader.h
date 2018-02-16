#ifndef __SHADER_H_
#define __SHADER_H_

#include "Glew/include/glew.h"
#include <fstream>
#include <vector>
#include <map>

using namespace std;

enum defaultShaders
{
	DEFAULTSHADER,
	ZBUFFERSHADER,

	NUMSHADERS
};

struct shaderInfo
{
	string name;
	GLuint vshaderID;
	GLuint fshaderID;
};

class Shader {
public:
	Shader();
	~Shader();

	bool loadShaders(const char* vshaderFile, const char* fshaderFile, string name);
	void cleanUp();

public:
	string defaultShaders[NUMSHADERS] = { "Default shader", "ZBuffer shader" };
	vector<shaderInfo*> shaders;
	map<string, GLuint> programs;

private:
	GLuint createShader(const char* shader, GLenum shaderType);
	bool readShader(const char* filename, GLenum shaderType, GLuint& id);
	bool linkShaders(shaderInfo* info);
	void initDefaultShaders();
};

#endif __SHADER_H_