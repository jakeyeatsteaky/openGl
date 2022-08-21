#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shaders.hpp"

void fb_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void keycallback(GLFWwindow* window, int key, int scancode, int action, int mods);

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

// Create and store pointer to vertex and fragment shader

const char* vertexShaderSource = 
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec3 aColor;\n"

	"out vec4 vertexColor;\n"
	"out vec3 multiColor;\n"
	"uniform float coords;\n"
	
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(-aPos.x, -aPos.y, aPos.z, 1.0) + vec4(-coords, -coords, coords, coords);\n"
		"multiColor = aColor;\n"
	"}\0";

const char* fragmentShaderSource = 
	"#version 330 core\n"
	"out vec4 FragColor;\n"

	"void main()\n"
	"{\n"
	"   FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
	"}\n\0";

const char* fragmentShaderSource2 =
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec3 multiColor;\n"

	"uniform vec4 outputColor;\n"

	"void main()\n"
	"{\n"
	"   FragColor = vec4(multiColor, 1.0f);\n"
	"}\n\0";

const char* fragmentShaderSource3 =
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec4 vertexColor;\n"
	"in vec3 multiColor;\n"

	"uniform vec4 outputColor2;\n"

	"void main()\n"
	"{\n"
	"   FragColor = vec4(multiColor, 1.0f);\n"
	"}\n\0";

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


	// Build and compile the Shader Program - Vertex and Fragment shaders must be linked to a Shader Program Object, which is then activated when rendering
	// Create Vertex and Fragment shaders first, then create Shader Program, then link

	const char* vertPath = "Shaders/vertShader.txt";
	const char* fragPath = "Shaders/fragshader.txt";

	Shader myShader(vertPath, fragPath);
	std::string vertShad = myShader.getVertShader();
	const char* vertexSourceCodeFromShaderObject = vertShad.c_str();

	std::string fragShad = myShader.getFragShader();
	const char* fragmentSourceCodeFromShaderObject = fragShad.c_str();

	// FOR WHEN I RETURN
	// I want my shader constructor to just store the shader source code
	// it stores the code as a string, so in order to use it in glShaderSource, I will need
	// to convert that member string into a const char* before passing it in.

	// from here I would like to
		// create a function to compile shaders
		// create a function to link shaders (create shader program) --> need to consider what to do in terms of resource management and when to delete the shader program
		// make program to use shader programs based on ID


	// Vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);		// create the shader
	glShaderSource(vertexShader, 1, &vertexSourceCodeFromShaderObject, NULL);					// Link the source code to the shader
	glCompileShader(vertexShader);										// compile the shader and check for errors
	int success;
	char vertexShaderInfoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, vertexShaderInfoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vertexShaderInfoLog << std::endl;
	}
	// Creating a shader:
		// initialize and create shader (glCreateShader)
		// compile source code for shader
		// error checking
			// glGetShaderIv

	// Fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSourceCodeFromShaderObject, NULL);
	glCompileShader(fragmentShader);
	int fragSuccess;
	char fragmentShaderInfoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragSuccess);
	if (!fragSuccess)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, fragmentShaderInfoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragmentShaderSource << std::endl;
	}

	unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader2);
	int fragSuccess2;
	char fragmentShaderInfoLog2[512];
	glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &fragSuccess2);
	if (!fragSuccess2)
	{
		glGetShaderInfoLog(fragmentShader2, 512, NULL, fragmentShaderInfoLog2);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragmentShaderSource2 << std::endl;
	}

	unsigned int fragmentShader3 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader3, 1, &fragmentShaderSource3, NULL);
	glCompileShader(fragmentShader3);
	int fragSuccess3;
	char fragmentShaderInfoLog3[512];
	glGetShaderiv(fragmentShader3, GL_COMPILE_STATUS, &fragSuccess3);
	if (!fragSuccess3)
	{
		glGetShaderInfoLog(fragmentShader3, 512, NULL, fragmentShaderInfoLog3);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragmentShaderSource3 << std::endl;
	}

	// Create shader program and link shaders
	unsigned int shaderProgram = glCreateProgram();					// create shader program
	glAttachShader(shaderProgram, vertexShader);					// attach both shaders to shader program
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);									// link shader program - links all attached shaders in the program specified in argument

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);		// check for linking errors
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, vertexShaderInfoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED1\n" << vertexShaderInfoLog << std::endl;
	}

	unsigned int shaderProgram2 = glCreateProgram();					// create shader program
	glAttachShader(shaderProgram2, vertexShader);					// attach both shaders to shader program
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);									// link shader program - links all attached shaders in the program specified in argument

	glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);		// check for linking errors
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram2, 512, NULL, vertexShaderInfoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED2\n" << vertexShaderInfoLog << std::endl;
	}

	unsigned int shaderProgram3 = glCreateProgram();					// create shader program
	glAttachShader(shaderProgram3, vertexShader);					// attach both shaders to shader program
	glAttachShader(shaderProgram3, fragmentShader3);
	glLinkProgram(shaderProgram3);									// link shader program - links all attached shaders in the program specified in argument

	glGetProgramiv(shaderProgram3, GL_LINK_STATUS, &success);		// check for linking errors
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram3, 512, NULL, vertexShaderInfoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED3\n" << vertexShaderInfoLog << std::endl;
	}

// Cleanup and delete shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShader2);
	glDeleteShader(fragmentShader3);


	// Setup vertex data, vertex buffers and configure vertex attributes
	float vertices1[] = {
	 0.0f, 0.5f, 0.0f,  // top right
	 0.0f, -0.5f, 0.0f,  // bottom right
	-0.5f, 0.5f, 0.0f,  // bottom left
	-0.5f, -0.5f, 0.0f   // top left  
		};

	float vertices2[]{
	 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
	0.0f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // top left  
	0.0f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 3,
		3,0,2
	};

	float vertices3[]{
	 -0.3f,  -0.3f, 0.0f,  1.0f, 0.0f, 0.0f, 
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

	// Second Element
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Triangle
	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);

	   
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
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);								// specify polygon mode

	// set Viewport
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	
	int maxAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributes);
	std::cout << "Max number of vertex attributes available: " << maxAttributes << std::endl;

	
	myShader.printShaderCode();

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

		// Draw calls to make shape
		int vertexPositionLocation = glGetUniformLocation(shaderProgram, "coords");
		glUseProgram(shaderProgram);							// Specify the shader program
		glUniform1f(vertexPositionLocation, blueSin2);
		glBindVertexArray(VAO[0]);
		glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);
		
		int vertexColorLocation2 = glGetUniformLocation(shaderProgram2, "outputColor");
		glUseProgram(shaderProgram2);							// Specify the shader program
		glUniform4f(vertexColorLocation2, 1.0f, 0.0f, 0.0f, 1.0);
		glBindVertexArray(VAO[1]);
		glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);	

		// using a uniform in shader program 3 from fragmentshadersourcecode3
		float time = glfwGetTime();
		float blueSin = (sin(time) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram3, "outputColor2");
		glUseProgram(shaderProgram3);							// Specify the shader program
		//glUniform4f(vertexColorLocation, 0.0f, 0.0f, blueSin, 1.0f);
		glBindVertexArray(VAO[2]);
		glDrawArrays(GL_TRIANGLES,0,3);

		// Swap buffers and check for events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// de-allocate all resources once they have performed their purpose
	/*glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(shaderProgram2);*/
	glDeleteVertexArrays(3, VAO);
	glDeleteBuffers(3, VBO);
	glDeleteBuffers(2, EBO);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(shaderProgram2);
	glDeleteProgram(shaderProgram3);
	
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










