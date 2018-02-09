#include "Shader.h"
#include "Globals.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <iostream>
#include <fstream>
#include <string>

Shader::Shader()
{
	initDefaultShaders();
}

Shader::~Shader() {}

GLuint Shader::loadShader(const char* shader, GLenum shaderType)
{
	GLuint shaderID;
	shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &shader, NULL);
	glCompileShader(shaderID);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		LOG("Shader compilation error: %s", infoLog);
	}

	return shaderID;
}

void Shader::linkShaders()
{
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vshaderID);
	glAttachShader(shaderProgram, fshaderID);
	glLinkProgram(shaderProgram);

	GLint success;
	GLchar infoLog;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, &infoLog);
		LOG("Shader link error: %s", infoLog);
	}
}

void Shader::initDefaultShaders()
{
	vshaderID = readShader("Assets/Shaders/vertshader.txt", GL_VERTEX_SHADER);
	fshaderID = readShader("Assets/Shaders/fragshader.txt", GL_FRAGMENT_SHADER);

	linkShaders();

	glUseProgram(shaderProgram);
}

void Shader::cleanUp() {}

GLuint Shader::readShader(char* filename, GLenum shaderType)
{
	ifstream file;
	file.open(filename, ios::in); // opens as ASCII!
	if (!file) "";

	string shaderText = "";
	string line;
	while (getline(file, line)) 
	{
		shaderText += line + "\n";
	}
	file.close();

	GLuint shaderID = loadShader(shaderText.c_str(), shaderType);
	return shaderID;
}