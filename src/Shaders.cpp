#include "Shaders.hpp"



Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
// 1. retreieve source code from a file path
	std::string vertCode;
	std::string fragCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit); // ensure fstream objects can throw exceptions
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// Open the Files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vertShaderStream, fragShaderStream;

		// Read the file's buffer contents into the stream
		vertShaderStream << vShaderFile.rdbuf();
		fragShaderStream << fShaderFile.rdbuf();

		// close file handlers
		vShaderFile.close();
		fShaderFile.close();

		// convert stream into a string
		vertCode = vertShaderStream.str();
		fragCode = fragShaderStream.str();
	}

	catch(std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_READ_UNSUCCESSFUL " << std::endl;
	}

	const char* vShaderCode = vertCode.c_str();
	const char* fShaderCode = fragCode.c_str();

	m_vertexShaderCode = vShaderCode;
	m_fragmentShaderCode = fShaderCode;

	// Source code in this object is stored as a string, but will need to be copied into const char* when used 
	// as shader source code
}


std::string Shader::getVertShader()
{
	return m_vertexShaderCode;
}

std::string Shader::getFragShader()
{
	return m_fragmentShaderCode;
}

void Shader::printShaderCode()
{
	std::cout << m_vertexShaderCode << std::endl;
	std::cout << "\n\n\n";
	std::cout << m_fragmentShaderCode << std::endl;
}




