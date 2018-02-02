#include "Shader.h"
#include "Globals.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

Shader::Shader()
{
	initDefaultShaders();
}

Shader::~Shader() {}

GLuint Shader::loadShader(const char* shader)
{
	GLuint shaderID;
	shaderID = glCreateShader(GL_VERTEX_SHADER);
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
	string vertShader = readShader("Assets/Shaders/vertshader.txt");
	const GLchar* vshaderFile = vertShader.c_str();
	
	string fragShader = readShader("Assets/Shaders/fragshader.txt");
	const GLchar* fshaderFile = fragShader.c_str();;

	vshaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vshaderID, 1, &vshaderFile, NULL);
	glCompileShader(vshaderID);

	fshaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fshaderID, 1, &fshaderFile, NULL);
	glCompileShader(fshaderID);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vshaderID, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		glGetShaderInfoLog(vshaderID, 512, NULL, infoLog);
		LOG("Shader compilation error: %s", infoLog);
	}

	glGetShaderiv(fshaderID, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		glGetShaderInfoLog(fshaderID, 512, NULL, infoLog);
		LOG("Shader compilation error: %s", infoLog);
	}
	else
	{
		linkShaders();
	}

	glUseProgram(shaderProgram);
}

void Shader::cleanUp() {}

string Shader::readShader(char* filename)
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

	return shaderText;
}

