#include "Shader.h"
#include "Globals.h"
#include <gl/GL.h>
#include <gl/GLU.h>

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
	//Load shaders from files to const char*

	const GLchar* vshaderFile = "#version 330 core\n"
		"layout (location = 0) in vec3 position;\n"
		"uniform mat4 model_matrix;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main() {\n"
		"gl_Position = projection * view * model_matrix * vec4(position, 1.0f); }\n\0"; 

	const GLchar* fshaderFile = "#version 330 core\n"
		"out vec4 color;\n"
		"void main() {\n"
		"color = vec4(1.0f, 0.f, 0.f, 0.f); }\n\0";

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