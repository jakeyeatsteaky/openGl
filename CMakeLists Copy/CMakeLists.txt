cmake_minimum_required(VERSION 3.10)

project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(LearnOpenGL build/src/main.cpp)

#target_include_directories(LearnOpenGL PRIVATE "C:/opencv/build/include")
target_include_directories(LearnOpenGL PRIVATE "C:/visualStudioCppLibraries/glfw-3.3.7.bin.WIN64/include")
target_include_directories(LearnOpenGL PRIVATE "C:/visualStudioCppLibraries/glm")
target_include_directories(LearnOpenGL PRIVATE "C:/visualStudioCppLibraries/imgui")
target_include_directories(LearnOpenGL PRIVATE "C:/visualStudioCppLibraries/imgui/backends")
target_include_directories(LearnOpenGL PRIVATE "C:/visualStudioCppLibraries/tinyobjloader")
target_include_directories(LearnOpenGL PRIVATE "C:/VulkanSDK/1.3.216.0/Include")
target_include_directories(LearnOpenGL PRIVATE "C:/visualStudioCppLibraries/stb")
target_include_directories(LearnOpenGL PRIVATE "C:/visualStudioCppLibraries/glad/include")
target_include_directories(LearnOpenGL PRIVATE "C:/visualStudioCppLibraries/Lua/include")



#target_link_directories(LearnOpenGL PRIVATE "C:/opencv/build/x64/vc15/lib")
target_link_directories(LearnOpenGL PRIVATE "C:/visualStudioCppLibraries/glfw-3.3.7.bin.WIN64/lib-vc2022")
target_link_directories(LearnOpenGL PRIVATE "C:/VulkanSDK/1.3.216.0/Lib")
target_link_directories(LearnOpenGL PRIVATE "C:/visualStudioCppLibraries/Lua")
target_link_libraries(LearnOpenGL "vulkan-1.lib")
target_link_libraries(LearnOpenGL "glfw3.lib")