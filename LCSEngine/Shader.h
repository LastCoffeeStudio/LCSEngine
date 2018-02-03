#ifndef __SHADER_H_
#define __SHADER_H_

#include "Glew/include/glew.h"
#include <fstream>

using namespace std;

class Shader {
public:
	Shader();
	~Shader();

	GLuint loadShader(const char* shader, GLenum shaderType);
	GLuint readShader(char* filename, GLenum shaderType);
	void linkShaders();
	void initDefaultShaders();
	void cleanUp();
public:
	GLuint vshaderID = 0;
	GLuint fshaderID = 0;
	GLuint shaderProgram = 0;
};

#endif __SHADER_H_