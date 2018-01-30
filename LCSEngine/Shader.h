#ifndef __SHADER_H_
#define __SHADER_H_

#include "Glew/include/glew.h"

class Shader {
public:
	Shader();
	~Shader();

	GLuint loadShader(const char* shader);
	void linkShaders();
	void initDefaultShaders();
	void cleanUp();
public:
	GLuint vshaderID = 0;
	GLuint fshaderID = 0;
	GLuint shaderProgram = 0;
};

#endif __SHADER_H_