#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include "ExceptionMacro.hpp"

#if defined(_WIN32) && defined(NDEBUG)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

//Data declaration section
static GLFWwindow* gWindow = NULL;

//Functions declaration section
void InitContext();
void InitGL();
void RunGameLoop();
void CleanUpGLFW();
std::shared_ptr<char> GetShaderCode(const char* path);
void KeyCallback(GLFWwindow* window, int key, int scan, int action,
	int mode);
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

//Entry point section
int main(int arg, char *args[]) 
{
	try 
	{
		InitContext();
		InitGL();
		RunGameLoop();
		CleanUpGLFW();
	}
	catch (const std::exception& exp) 
	{
		std::cout << exp.what() << std::endl;
	}
	return 0;
}

//Function defines section

void InitContext() 
{
	if (!glfwInit()) 
	{
		LT_RUNTIME_ERR("Can't init GLFW");
	}
	const GLFWvidmode* vidmode = glfwGetVideoMode(
		glfwGetPrimaryMonitor());
	glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
	glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);
	glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
	glfwWindowHint(GLFW_REFRESH_RATE, vidmode->refreshRate);
	gWindow = glfwCreateWindow(500, 500, "OpenGL Comeback",
		NULL, NULL);
	if (!gWindow) 
	{
		glfwTerminate();
		LT_RUNTIME_ERR("Can't create GLFW window!");
	}
	glfwMakeContextCurrent(gWindow);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
	{
		glfwDestroyWindow(gWindow);
		glfwTerminate();
		LT_RUNTIME_ERR("Can't load GL extension!");
	}
	glfwSwapInterval(1);
	glfwSetKeyCallback(gWindow, &KeyCallback);
	glfwSetFramebufferSizeCallback(gWindow, &FramebufferSizeCallback);
}

void InitGL() 
{
	//Init OpenGL section
	glClearColor(0, 0, 0, 1);
}

void RunGameLoop() 
{
	while (!glfwWindowShouldClose(gWindow)) 
	{
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glDisable(GL_DEPTH_TEST);
		glfwPollEvents();
		glfwSwapBuffers(gWindow);
	}
}

void CleanUpGLFW() 
{
	glfwDestroyWindow(gWindow);
	glfwTerminate();
}

std::shared_ptr<char> GetShaderCode(const char* path) 
{
	std::ifstream file{ path, std::ios::in | std::ios::binary };
	if (!file.is_open()) 
	{
		assert(false && "File not found");
		LT_RUNTIME_ERR("File not found!");
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
	int mode) 
{
	if (key == GLFW_KEY_ESCAPE) 
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) 
{
	glViewport(0, 0, width, height);
}
