#include <WinSock2.h>
#include <ws2tcpip.h>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ExceptionMacro.hpp"
#include "ModelTool.hpp"
#include "BasicShader.hpp"

#define DEFAULT_PORT "1234"
#define DEFAULT_BUFLEN 2048

#pragma comment(lib, "Ws2_32.lib")

#if defined(_WIN32) && defined(NDEBUG)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

//Data declaration section
static GLFWwindow* g_Window = NULL;
static unsigned g_Vao;
static unsigned g_NumVertices;
static std::shared_ptr<BasicShader> g_BasicShader;

//Functions declaration section
void InitContext();
void InitGL();
void InitShader();
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
	g_Window = glfwCreateWindow(700, 700, "OpenGL Comeback",
		NULL, NULL);
	if (!g_Window) 
	{
		glfwTerminate();
		LT_RUNTIME_ERR("Can't create GLFW window!");
	}
	glfwMakeContextCurrent(g_Window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
	{
		glfwDestroyWindow(g_Window);
		glfwTerminate();
		LT_RUNTIME_ERR("Can't load GL extension!");
	}
	glfwSwapInterval(1);
	glfwSetKeyCallback(g_Window, &KeyCallback);
	glfwSetFramebufferSizeCallback(g_Window, &FramebufferSizeCallback);
}

void InitGL() 
{
	InitShader();
	glClearColor(0, 0, 0, 1);
	//Object
	std::vector<float> coords;
	std::vector<float> normals;
	ModelTool modelReader{"../model/sword.fbx"};
	modelReader.loadModelCN(coords, normals);
	std::size_t coordBytes = coords.size() * sizeof(float);
	std::size_t normalBytes = normals.size() * sizeof(float);
	std::size_t totalSize = coordBytes + normalBytes;
	g_NumVertices = coords.size() / 3;
	unsigned int vbo;
	glGenVertexArrays(1, &g_Vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(g_Vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, totalSize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, coordBytes, coords.data());
	glBufferSubData(GL_ARRAY_BUFFER, coordBytes, normalBytes, normals.data());
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)coordBytes);
	glEnableVertexAttribArray(0); //vertex coord
	glEnableVertexAttribArray(1); //vertex normal
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void InitShader() 
{
	g_BasicShader = std::make_shared<BasicShader>(
		"../shader/basic.vt",
		"../shader/basic.fr"
		);
}

void RunGameLoop() 
{
	int iResult;
	struct addrinfo* result = NULL, * ptr = NULL, hints;
	WSADATA wsaData;
	SOCKET listenSocket = INVALID_SOCKET;
	SOCKET clientSocket = INVALID_SOCKET;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return;
	}

	//Get socket information
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return;
	}

	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (listenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	// Setup the TCP listening socket
	iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
		return;
	}

	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return;
	}

	// Accept a client socket
	clientSocket = accept(listenSocket, NULL, NULL);
	if (clientSocket == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(clientSocket);
		WSACleanup();
		return;
	}

	int recvbuflen = sizeof(float) * 9;
	char recvbuf[sizeof(float) * 9];

	while (!glfwWindowShouldClose(g_Window)) 
	{
		//Socket
		iResult = recv(clientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			float* matrix = (float*)recvbuf;

			//Graphics
			glEnable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glBindVertexArray(g_Vao);
			g_BasicShader->UseProgram();
			//math
			glm::mat4 lookAtMat = glm::lookAt(
				glm::vec3{ 0.f, -2.f, 3.f },
				glm::vec3{ 0.f, 0.f, 2.f },
				glm::vec3{ 0.f, 1.f, 0.f }
			);
			glm::mat4 projectionMat = glm::perspective(
				glm::radians(90.f), 1.f, 0.024f, 1000.f
			);
			static float angle = 0;
			glm::mat4 rotationMat{
				matrix[0], matrix[3], matrix[6], 0.f,
				matrix[1], matrix[4], matrix[7], 0.f,
				matrix[2], matrix[5], matrix[8], 0.f,
				0.f, 0.f, 0.f, 1.f
			};
			glm::mat4 transformMat = projectionMat * lookAtMat * rotationMat;
			glm::vec3 lightDir = glm::normalize(glm::vec3{ -1.f, 0.f, -2.f });
			glm::vec3 lightColor{ 1.f, 1.f, 1.f };
			glm::vec3 defaultColor{ 1.f, 0.f, 1.f };
			//uniform
			glUniformMatrix4fv(
				g_BasicShader->u_TransformMat(),
				1, GL_FALSE, (float*)& transformMat
			);
			glUniformMatrix4fv(
				g_BasicShader->u_RotationMat(),
				1, GL_FALSE, (float*)& rotationMat
			);
			glUniform1f(g_BasicShader->u_AmbientFactor(), 0.2f);
			glUniform3fv(
				g_BasicShader->u_LightDir(), 1,
				(float*)& lightDir
			);
			glUniform3fv(
				g_BasicShader->u_LightColor(), 1,
				(float*)& lightColor
			);
			glUniform3fv(
				g_BasicShader->u_DefaultColor(), 1,
				(float*)& defaultColor
			);
			glDrawArrays(GL_TRIANGLES, 0, g_NumVertices);
			g_BasicShader->UnUseProgram();
			glBindVertexArray(0);
			glDisable(GL_DEPTH_TEST);
			glfwPollEvents();
			glfwSwapBuffers(g_Window);
		}
	}

	// shutdown the connection since we're done
	iResult = shutdown(clientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(clientSocket);
		WSACleanup();
		return;
	}

	// cleanup
	closesocket(clientSocket);
	WSACleanup();
}

void CleanUpGLFW() 
{
	glfwDestroyWindow(g_Window);
	glfwTerminate(); 
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
