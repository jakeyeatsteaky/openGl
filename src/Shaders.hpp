#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
	unsigned int programID;

	Shader(const char* vertexPath, const char* fragmentPath); // Constructor reads and builds shaders

	std::string getVertShader();
	std::string getFragShader();

	void printShaderCode();

	void use(); // uses and activates the shader program

	void setBool(const std::string& name, bool value) const; // query a uniform location and read value
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

private:
	std::string m_vertexShaderCode;
	std::string m_fragmentShaderCode;

};






#endif
