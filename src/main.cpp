#include <iostream>
#include <glad/glad.h>
#include <vector>
#include <GLFW/glfw3.h>
#include "Shaders.hpp"

extern "C"
{
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
}

#pragma comment(lib, "liblua54.a")

void fb_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void keycallback(GLFWwindow* window, int key, int scancode, int action, int mods);

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

bool CheckLua(lua_State* L, int r);

int main()
{
	std::string cmd = "a = 1";
	std::string cmd2 = "b = a + 12 * 79 + math.sin(23.7)";

	lua_State *L = luaL_newstate();											// L represents a single instance of a lua virtual machine
	luaL_openlibs(L);

	if(CheckLua(L, luaL_dostring(L, cmd.c_str())));			
	{
		lua_getglobal(L, "a");												// pushes data box with a to the top of the stack
		if (lua_isnumber(L, -1))											// queries the top of the stack (index -1)
		{
			float a_in_cpp = static_cast<float>(lua_tonumber(L, -1));
			std::cout << "a_in_cpp: " << a_in_cpp << std::endl;

		}
	}

	if(CheckLua(L, luaL_dostring(L, cmd2.c_str())));			
	{
		lua_getglobal(L, "b");
		if (lua_isnumber(L, -1))
		{
			float b_in_cpp = static_cast<float>(lua_tonumber(L, -1));
			std::cout << "b_in_cpp: " << b_in_cpp << std::endl;
		}
	}


	if (CheckLua(L, luaL_dofile(L, "src/practiceLua.lua")))
	{
		lua_getglobal(L, "testLua");
		if (lua_isnumber(L, -1))
		{
			float testLua = static_cast<float>(lua_tonumber(L, -1));
			std::cout << "from file practiceLua.lua: " << testLua << std::endl;
		}
	}

	int idx1, idx2, idx3, idx4;

	if (CheckLua(L, luaL_dofile(L, "src/practiceLua.lua")))
	{
		lua_getglobal(L, "idx1");
		if (lua_isnumber(L, -1))
		{
			idx1 = static_cast<int>(lua_tonumber(L, -1));
		}

		lua_getglobal(L, "idx2");
		if (lua_isnumber(L, -1))
		{
			idx2 = static_cast<int>(lua_tonumber(L, -1));
		}

		lua_getglobal(L, "idx3");
		if (lua_isnumber(L, -1))
		{
			idx3 = static_cast<int>(lua_tonumber(L, -1));
		}

		lua_getglobal(L, "idx4");
		if (lua_isnumber(L, -1))
		{
			idx4 = static_cast<int>(lua_tonumber(L, -1));
		}
	}

	if (CheckLua(L, luaL_dofile(L, "src/practiceLua.lua")))
	{
		lua_getglobal(L, "Add");
		if (lua_isfunction(L, -1))
		{
			lua_pushnumber(L, 0.3f);
			lua_pushnumber(L, 0.5f);

			if (CheckLua(L, lua_pcall(L, 2, 1, 0)))
			{
				std::cout << "Lua: " << static_cast<float>(lua_tonumber(L, -1)) << std::endl;
			}

		}
	}



	// The Lua Stack: individual elements of the stack is indexed.  Each element on the stack contains the data, as well as information describing the data.  so when calling lua_isnumber, you interrogate the box to see what the data holds (so C++ knows the type).

	// Stack is indexed from bottom up starting with 1.  0 = empty stack
	// Can also be indexed relative to the top of the stack.  Top = -1, .... all the way down.  when querying the -1 up there, its specifying whats on top of the stack.


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
	 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
	0.0f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // top left  
	0.0f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f
	};

	unsigned int indices[] = {
		idx1, idx2, idx4,
		idx4, idx1, idx3
	};

	std::cout << "indices: [" << idx1 << ", " << idx2 << ", " << idx3 << ", " << idx4 << "]" << std::endl;

	float R1, G1;

	if (CheckLua(L, luaL_dofile(L, "src/practiceLua.lua")))
	{
		lua_getglobal(L, "color");
		if (lua_istable(L, -1))
		{
			lua_pushstring(L, "R1");
			lua_gettable(L, -2);
			R1 = lua_tonumber(L, -1);
			lua_pop(L, 1);

			lua_pushstring(L, "G1");
			lua_gettable(L, -2);
			G1 = lua_tonumber(L, -1);
			lua_pop(L, -1);

		}
	}

	// Explaining the pattern above for lua:
	// After the first call to "color", we know that the box "color" was placed on top of the stack.
	// Query lua to see if the position -1 on the stack is a table, and if so
	// Push the string (key) "G1" to the top of the stack.  Now since thats on top, we query the table at position -2
	// on the stack to retrieve the value at key G1
	// convert it to a number.
	// Remove the string "G1" from the top of the stack so it goes back to looking like a regular table

	float vertices3[]{
	 -0.3f,  -0.3f, 0.0f,  R1, 0.0f,0.0f, 
	 0.3f, -0.3f, 0.0f,    0.0f, G1, 0.0f,
	 0.0f, 0.3f, 0.0f,     0.0f, 0.0f, G1
	
	};


	lua_close(L);

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



bool CheckLua(lua_State* L, int r)
{
	if (r != LUA_OK)
	{
		std::string errormsg = lua_tostring(L, -1);
		std::cout << errormsg << std::endl;
		return false;
	}
	return true;
}







