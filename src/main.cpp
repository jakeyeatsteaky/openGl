#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <glad/glad.h>
#include <vector>
#include <GLFW/glfw3.h>
#include "Shaders.hpp"
#include "stb_image.h"


void fb_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void keycallback(GLFWwindow* window, int key, int scancode, int action, int mods);

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

unsigned int generateTexture(std::string textureName);

int main()
{
	// First initialize the glfw library
	glfwInit();
	// Then apply configuration settings using glfwWindowHint(*,*)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// Create the window that we will be rendering to
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Learning Open GL", NULL, NULL);
	
	// Error check that the window exists
	if (window == NULL)
	{
		std::cout << "Failed to create the GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}


	glfwSetKeyCallback(window, keycallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

	// Make the context of the specified window (this one) current.  Thread can have 1 context at a time
	glfwMakeContextCurrent(window);

	// Tell GLFW to call viewport size callback every time window is resized
	glfwSetFramebufferSizeCallback(window, fb_size_callback);

	// Initilize GLAD in order to use OpenGL functions, by passing to glad the function to find the address of the function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	const char* vertPath = "Shaders/vertShader.txt";
	const char* fragPath = "Shaders/fragshader.txt";
	const char* fragPath2 = "Shaders/fragshader2.txt";
	const char* fragPath3 = "Shaders/fragshader3.txt";


	Shader myShader(vertPath, fragPath);
	unsigned int shaderProgram = myShader.compileAndCreateShaderProgram();

	Shader myShader2(vertPath, fragPath2);
	unsigned int shaderProgram2 = myShader2.compileAndCreateShaderProgram();

	Shader myShader3(vertPath, fragPath3);
	unsigned int shaderProgram3 = myShader3.compileAndCreateShaderProgram();

	// Setup vertex data, vertex buffers and configure vertex attributes
	float vertices1[] = {
	 0.0f, 0.5f, 0.0f,  // top right
	 0.0f, -0.5f, 0.0f,  // bottom right
	-0.5f, 0.5f, 0.0f,  // bottom left
	-0.5f, -0.5f, 0.0f   // top left  
		};

	float vertices2[]{
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
	 0.0f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  0.0f, 1.0f,  
	 0.0f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  1.0f, 0.0f
	}; //EBO with the texture assigned

	unsigned int indices[] = 
	{
		0, 1, 3,
		3, 0, 2
	};

	float vertices3[]{
	 -0.3f,  -0.3f, 0.0f,  1.0f, 0.0f,0.0f, 
	 0.3f, -0.3f, 0.0f,    0.0f, 1.0f, 0.0f,
	 0.0f, 0.3f, 0.0f,     0.0f, 0.0f, 1.0f
	
	};

	unsigned int VAO[3], VBO[3], EBO[2];
	glGenBuffers(3, VBO);
	glGenBuffers(2, EBO);
	glGenVertexArrays(3, VAO);

	// First Element
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// Example of querying location of vertex attribute (aPos) from vertex shader
	GLuint num;
	num = glGetAttribLocation(shaderProgram, "aPos");
	glEnableVertexAttribArray(num);
	std::cout << "attriblocation: " << num << std::endl;

	// Second Element - updating another vertexattrippointer for added vertex data for the texture coords
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Triangle
	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
  
	// Drawing triangles with multiple VAO/VBO steps
	// Create VAO and VBOs 
	// One Triangle at time: 
		// Bind VAO at respective index position
		// Bind matching VBO at respective index
		// Copy vertex info to VBO
		// assign vertex attributes
		// enable vertex attrivutes
	// do the other triangle
	// THEN WHEN ITS TIME TO DRAW
	// Specify shader program
	// Bind specific VAO index -> gldrawarrays
	// Bind other index -> gldrawthosearrays
	// Unbind VBO and then unbind VAO (same as binding it to 0)

	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);								// specify polygon mode

	// set Viewport
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	
	int maxAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributes);
	std::cout << "Max number of vertex attributes available: " << maxAttributes << std::endl;


	unsigned int texture = generateTexture("container.jpg");

	// Set render loop - poll for events and swap buffers
	while (!glfwWindowShouldClose(window))
	{
		// check for keyboard or mouse input
		processInput(window);
		
		// rendering commands
		glClearColor(0.6f, 0.15f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float time2 = glfwGetTime();
		float blueSin2 = (sin(time2) / 2.0f) + 0.5f;

		myShader.use();											// use shader program
		myShader.setUniformFloat("coords", blueSin2);			// uniform location and value
		glBindVertexArray(VAO[0]);
		glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);
		
		glm::vec4 outputColor = glm::vec4(1.0, 0.0, 0.0, 1.0);
		myShader2.use();
		myShader2.setUniformVec4f("outputColor", outputColor);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO[1]);
		glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);	

		myShader3.use();
		// shader3 uses vertex input data for color
		glBindVertexArray(VAO[2]);
		glDrawArrays(GL_TRIANGLES,0,3);

		// Swap buffers and check for events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(3, VAO);
	glDeleteBuffers(3, VBO);
	glDeleteBuffers(2, EBO);
	myShader.deleteShaderProgram();
	myShader2.deleteShaderProgram();
	myShader3.deleteShaderProgram();
	
	// Cleanup
	glfwTerminate();

}

void fb_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

//Actions: Pressed, Release, Constant
void keycallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE)
		switch (action)
		{
		case (GLFW_PRESS):
			std::cout << key << " has been pressed" << std::endl;
			break;

		case (GLFW_RELEASE):
			std::cout << key << " has been release" << std::endl;
			break;

		case (GLFW_REPEAT):
			std::cout << key << " has been held down" << std::endl;
			break;

		}
}

unsigned int generateTexture(std::string textureName)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// wrapping and filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//load and generate texture
	int width, height, numChannels;
	std::string path = "Textures/" + textureName;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << textureName << " was successfully loaded." << std::endl;
	}
	else
	{
		std::cout << " failed to load texture: " << textureName << std::endl;
	}

	stbi_image_free(data);

	return texture;
}







