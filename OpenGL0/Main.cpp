#include <iostream>
#include <fstream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cassert>

void KeyCallback(GLFWwindow* window, int key, int scan, int action,
	int mode);

void n_glShaderSource(GLuint shader, const char* source, const GLint* length) {
	std::ifstream sourceFile(source, std::ios::in|std::ios::binary);
	if (!sourceFile.is_open()) {
		
	}
	glShaderSource(shader, 1, NULL, length);
	sourceFile.close();
}

int main(int arg, char *args[]) {
	std::cout << R""(
	 fefef
fef)"" << std::endl;
	
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
	glViewport(0, 0, 500, 500);
	glClearColor(1, 0, 1, 1);
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

