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

unsigned int Shader::compileAndCreateShaderProgram()
{
	int success;
	char infoLog[512];
	
	// Vertex Shaders
	std::string vertShader = this->m_vertexShaderCode;
	const char* vertSource = vertShader.c_str();

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);		// create the shader
	glShaderSource(vertexShader, 1, &vertSource, NULL);					// link shader and source code
	glCompileShader(vertexShader);										// compile
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);			// Error check
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Fragment shader
	std::string fragShader = this->m_fragmentShaderCode;
	const char* fragSource = fragShader.c_str();

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);		// create the shader
	glShaderSource(fragmentShader, 1, &fragSource, NULL);					// link shader and source code
	glCompileShader(fragmentShader);										// compile
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);			    // Error check
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	// Shader program and Link shaders

	unsigned int shaderProgram = glCreateProgram();							// initial create shader program	
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);							// attach shaders to program
	glLinkProgram(shaderProgram);											// link program

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED1\n" << infoLog << std::endl;
	}

	// Clean Up resources
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	m_shaderProgram = shaderProgram;

	std::cout << "Shader program: " << m_shaderProgram << std::endl;

	return shaderProgram;

}

void Shader::use()
{
	glUseProgram(this->m_shaderProgram);
}

void Shader::setUniformBool(const std::string& uniformName, bool value) const
{
	int uniformLocation = glGetUniformLocation(m_shaderProgram, uniformName.c_str());
	glUniform1i(uniformLocation, value);
}

void Shader::setUniformInt(const std::string& uniformName, int value) const
{
	int uniformLocation = glGetUniformLocation(m_shaderProgram, uniformName.c_str());
	glUniform1i(uniformLocation, value);
}


void Shader::setUniformFloat(const std::string& uniformName, float value) 
{
	int uniformLocation = glGetUniformLocation(m_shaderProgram, uniformName.c_str());
	glUniform1f(uniformLocation, value);
}

void Shader::setUniformVec4f(const std::string& uniformName, glm::vec4 value) 
{
	int uniformLocation = glGetUniformLocation(m_shaderProgram, uniformName.c_str());
	glUniform4f(uniformLocation, value.x, value.y, value.z, value.w);
}

void Shader::deleteShaderProgram()
{

	std::cout << "Deleting shader program: " << this->m_shaderProgram << std::endl;
	glDeleteProgram(m_shaderProgram);
}
