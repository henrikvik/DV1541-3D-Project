#include "Shader.h"
#include <fstream>
#include <stdio.h>
#include <glm\gtc\type_ptr.hpp>
#include <string>

GLuint Shader::activeProgram = GL_NONE;

GLuint Shader::loadShader(GLenum shaderType, const GLchar * filePath)
{
	GLuint shader = glCreateShader(shaderType);

	std::ifstream file(filePath);
	std::string text = { std::istreambuf_iterator<char>(file),  std::istreambuf_iterator<char>() };
	file.close();

	if (!file) printf("file '%s' does not exist\n", filePath);

	const GLchar * textPtr = text.c_str();
	glShaderSource(shader, 1, &textPtr, NULL);
	glCompileShader(shader);


	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		GLchar shaderLog[512] = { NULL };
		glGetShaderInfoLog(shader, 512, NULL, shaderLog);
		throw shaderLog;
	}

	return shader;
}


Shader::Shader(const char * vertexPath,
			   const char * fragmentPath)
{
	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexPath);
	GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentPath);

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLint success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLchar programLog[512] = { NULL };
		glGetProgramInfoLog(program, 512, NULL, programLog);
		throw programLog;
	}
}

Shader::Shader(const char * vertexPath, const char * geometryPath, const char * fragmentPath)
{
	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexPath);
	GLuint geometryShader = loadShader(GL_GEOMETRY_SHADER, geometryPath);
	GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentPath);

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, geometryShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	glDetachShader(program, vertexShader);
	glDetachShader(program, geometryShader);
	glDetachShader(program, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);

	GLint success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLchar programLog[512] = { NULL };
		glGetProgramInfoLog(program, 512, NULL, programLog);
		throw programLog;
	}
}

Shader::Shader(const char * vertexPath, const char * controlPath, const char * evaulationPath, const char * fragmentPath)
{
	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexPath);
	GLuint controlShader = loadShader(GL_TESS_CONTROL_SHADER, controlPath);
	GLuint evaluationShader = loadShader(GL_TESS_EVALUATION_SHADER, evaulationPath);
	GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentPath);

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, controlShader);
	glAttachShader(program, evaluationShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	glDetachShader(program, vertexShader);
	glDetachShader(program, controlShader);
	glDetachShader(program, evaluationShader);
	glDetachShader(program, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(controlShader);
	glDeleteShader(evaluationShader);
	glDeleteShader(fragmentShader);

	GLint success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLchar programLog[512] = { NULL };
		glGetProgramInfoLog(program, 512, NULL, programLog);
		throw programLog;
	}
}

Shader::~Shader()
{
	glDeleteProgram(program);
}

void Shader::use()
{
	glUseProgram(program);
	activeProgram = program;
}

GLint Shader::getUniform(const GLchar * name)
{
	if (activeProgram != program)
		throw printf("cant get uniform, shader not active");

	GLint location = glGetUniformLocation(program, name);

	if (location == -1)
		printf("cant find uniform '%s' in shader nr %d\n", name, program);

	return location;
}

void Shader::setUniform(const GLchar * name, const int & value)
{
	glUniform1i(getUniform(name), value);
}

void Shader::setUniform(const GLchar * name, const float & value)
{
	glUniform1f(getUniform(name), value);
}

void Shader::setUniform(const GLchar * name, const vec2 & vector)
{
	glUniform2fv(getUniform(name), 1, value_ptr(vector));
}

void Shader::setUniform(const GLchar * name, const vec3 & vector)
{
	glUniform3fv(getUniform(name), 1, value_ptr(vector));
}

void Shader::setUniform(const GLchar * name, const mat4 & matrix)
{
	glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, value_ptr(matrix));
}

void Shader::setTexture2D(int id, const GLchar * name, GLuint texture)
{
	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_2D, texture);
	setUniform(name, id);
}
