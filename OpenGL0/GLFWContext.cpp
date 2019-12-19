#include "GLFWContext.hpp"

GLFWContext& GLFWContext::GetInstance()
{
	static GLFWContext instance;
	return instance;
}
