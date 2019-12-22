#include <iostream>
#include <fstream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <stdexcept>
#include <sstream>

#ifdef _WIN32
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif


#define _RUNTIME_ERROR(message) \
	std::stringstream sstream;\
	sstream << __FILE__ << ", " << __LINE__ << ": " << message; \
	throw std::runtime_error(sstream.str())

#define _EXCEPTION(message) \
	std::stringstream sstream;\
	sstream << __FILE__ << ", " << __LINE__ << ": " << message; \
	throw std::exception(sstream.str())

std::shared_ptr<char> GetShaderCode(const char* path) {
	std::ifstream file{ path, std::ios::in | std::ios::binary };
	if (!file.is_open()) {
		assert(false && "File not found");
		_RUNTIME_ERROR("File not found!");
	}
	file.seekg(0, file.end);
	std::uint32_t fileSize = file.tellg();
	file.seekg(0, file.beg);
	char* data = new char[fileSize + 1];
	file.read(data, fileSize);
	data[fileSize] = NULL;
	return std::shared_ptr<char>(data);
}

void KeyCallback(GLFWwindow* window, int key, int scan, int action,
	int mode);
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

int main(int arg, char *args[]) {
	
	if (!glfwInit()) {
		std::cout << "GLFW init error!" << std::endl;
		return -1;
	} 
	const GLFWvidmode* vidmode = glfwGetVideoMode(
		glfwGetPrimaryMonitor());
	glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
	glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);
	glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
	glfwWindowHint(GLFW_REFRESH_RATE, vidmode->refreshRate);
	GLFWwindow* window = glfwCreateWindow(500, 500, "OpenGL Comeback",
		NULL, NULL);	
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, &KeyCallback);
	glfwSetFramebufferSizeCallback(window, &FramebufferSizeCallback);
	glClearColor(0, 0, 0, 1);
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void KeyCallback(GLFWwindow* window, int key, int scan, int action,
	int mode) {
	if (key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, true);
	}
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
