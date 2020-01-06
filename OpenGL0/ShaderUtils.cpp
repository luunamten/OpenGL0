#include <fstream>
#include <glad/glad.h>
#include "ExceptionMacro.hpp"
#include "ShaderUtils.hpp"

std::shared_ptr<char> GetShaderInfoLog(unsigned int shader) {
	int compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if (!compileStatus)
	{
		int logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0)
		{
			char* log = new char[logLength];
			glGetShaderInfoLog(shader, logLength, NULL, log);
			return std::shared_ptr<char>(log);
		}
	}
	return nullptr;
}

std::shared_ptr<char> GetProgramInfoLog(unsigned int program) {
	int linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if (!linkStatus)
	{
		int logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0)
		{
			char* log = new char[logLength];
			glGetProgramInfoLog(program, logLength, NULL, log);
			return std::shared_ptr<char>(log);
		}
	}
	return nullptr;
}

std::shared_ptr<char> GetShaderCode(const char* path)
{
	std::ifstream file{ path, std::ios::in | std::ios::binary };
	if (!file.is_open())
	{
		LT_ASSERT(false, "File not found");
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