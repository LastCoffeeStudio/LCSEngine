#include "Shader.h"
#include "Globals.h"
#include "Glew/include/glew.h"
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

bool Shader::loadShaders(const char* vshaderFile, const char* fshaderFile, string name)
{
	if (programs.find(name) != programs.end())
	{
		LOG("Failed to create shader. This shader name already exists");
		return false;
	}
	shaderInfo* info = new shaderInfo();
	info->name = name;

	if (!readShader(vshaderFile, GL_VERTEX_SHADER, info->vshaderID))
	{
		LOG("Failed to create shader. Couldn't read vertex shader");
		return false;
	}
	if (!readShader(fshaderFile, GL_FRAGMENT_SHADER, info->fshaderID))
	{
		LOG("Failed to create shader. Couldn't read fragment shader");
		return false;
	}

	if (!linkShaders(info))
	{
		LOG("Couldn't create program");
		return false;
	}

	shaders.push_back(info);

	return true;
}

bool Shader::readShader(const char* filename, GLenum shaderType, GLuint& id)
{
	bool ret = false;
	ifstream file;
	file.open(filename, ios::in); // opens as ASCII!
	if (file.is_open())
	{
		if (!file) "";

		string shaderText = "";
		string line;
		while (getline(file, line))
		{
			shaderText += line + "\n";
		}
		file.close();

		id = createShader(shaderText.c_str(), shaderType);
		ret = true;
	}
	else
	{
		LOG("File couldn't be opened");
	}

	return ret;
}

GLuint Shader::createShader(const char* shader, GLenum shaderType)
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

bool Shader::linkShaders(shaderInfo* info)
{
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, info->vshaderID);
	glAttachShader(shaderProgram, info->fshaderID);
	glLinkProgram(shaderProgram);

	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		LOG("Shader link error: %s", infoLog);
		return false;
	}
	programs[info->name] = shaderProgram;

	return true;
}

void Shader::initDefaultShaders()
{
	loadShaders("Assets/Shaders/vertshader.txt", "Assets/Shaders/fragshader.txt", "Default shader");
	loadShaders("Assets/Shaders/vertshader.txt", "Assets/Shaders/fragshaderZbuffer.txt", "ZBuffer shader");
	loadShaders("Assets/Shaders/vertshaderDiffuseLight.txt", "Assets/Shaders/fragshaderDiffuseLight.txt", "Diffuse shader");
	loadShaders("Assets/Shaders/vertshaderSpecularLight.txt", "Assets/Shaders/fragshaderSpecularLight.txt", "Specular shader");
	loadShaders("Assets/Shaders/vertshaderPixelLight.txt","Assets/Shaders/fragshaderPixelLight.txt","Pixel Light shader");
	glUseProgram(programs["Default shader"]);
}

void Shader::cleanUp()
{
	for (vector<shaderInfo*>::iterator it = shaders.begin(); it != shaders.end(); ++it)
	{
		glDetachShader(programs[(*it)->name], (*it)->vshaderID);
		glDetachShader(programs[(*it)->name], (*it)->fshaderID);
		glDeleteShader((*it)->vshaderID);
		glDeleteShader((*it)->fshaderID);
	}
}
