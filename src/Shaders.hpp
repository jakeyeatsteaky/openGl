#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath); // Constructor reads and builds shaders

	std::string getVertShader();
	std::string getFragShader();

	unsigned int compileAndCreateShaderProgram();
	void use();

	void printShaderCode();

	void setUniformBool(const std::string& name, bool value) const; // query a uniform location and read value
	void setUniformInt(const std::string& name, int value) const;
	void setUniformFloat(const std::string& name, float value);
	void setUniformVec4f(const std::string& name, glm::vec4 value);

	void deleteShaderProgram();

private:
	std::string m_vertexShaderCode;
	std::string m_fragmentShaderCode;
	unsigned int m_shaderProgram;
};






#endif
