#include "BasicShader.hpp"
#include <glad/glad.h>
#include "ShaderUtils.hpp"
#include "ExceptionMacro.hpp"

BasicShader::BasicShader(const char* vtPath, const char* frPath):
	m_Program(0),
	mu_TransformMat(-1),
	mu_RotationMat(-1),
	mu_AmbientFactor(-1),
	mu_LightDir(-1),
	mu_LightColor(-1)
{
	InitShader(vtPath, frPath);
}

BasicShader::~BasicShader()
{
	glDeleteProgram(m_Program);
}

void BasicShader::InitShader(const char* vtPath, const char* frPath)
{
	//Vertex shader
	int vtsd = glCreateShader(GL_VERTEX_SHADER);
	std::shared_ptr<char> vtsdCode = GetShaderCode(vtPath);
	const char* const vtsdSource[] =
	{
		vtsdCode.get()
	};
	glShaderSource(vtsd, 1, vtsdSource, NULL);
	glCompileShader(vtsd);
	std::shared_ptr<char> compileOrLinkLog = GetShaderInfoLog(vtsd);
	char* log = compileOrLinkLog.get();
	if (log)
	{
		LT_ASSERT(false, log);
		LT_RUNTIME_ERR(log);
	}

	//Fragment shader
	int frsd = glCreateShader(GL_FRAGMENT_SHADER);
	std::shared_ptr<char> frsdCode = GetShaderCode(frPath);
	const char* const frsdSource[] =
	{
		frsdCode.get()
	};
	glShaderSource(frsd, 1, frsdSource, NULL);
	glCompileShader(frsd);
	compileOrLinkLog = GetShaderInfoLog(frsd);
	log = compileOrLinkLog.get();
	if (log)
	{
		LT_ASSERT(false, log);
		LT_RUNTIME_ERR(log);
	}

	//Program
	m_Program = glCreateProgram();
	glAttachShader(m_Program, vtsd);
	glAttachShader(m_Program, frsd);
	glLinkProgram(m_Program);
	compileOrLinkLog = GetProgramInfoLog(m_Program);
	log = compileOrLinkLog.get();
	if (log)
	{
 		LT_ASSERT(false, log);
		LT_RUNTIME_ERR(log);
	}

	glDetachShader(m_Program, vtsd);
	glDetachShader(m_Program, frsd);
	glDeleteShader(vtsd);
	glDeleteShader(frsd);
	glReleaseShaderCompiler();

	//Get uniform indices
	//From vertex shader
	mu_TransformMat = glGetUniformLocation(m_Program, "u_TransformMat");
	mu_RotationMat = glGetUniformLocation(m_Program, "u_RotationMat");
	//From fragment shader
	mu_AmbientFactor = glGetUniformLocation(m_Program, "u_AmbientFactor");
	mu_LightDir = glGetUniformLocation(m_Program, "u_LightDir");
	mu_LightColor = glGetUniformLocation(m_Program, "u_LightColor");
	
	std::cout << mu_TransformMat << std::endl;
	std::cout << mu_RotationMat << std::endl;
	std::cout << mu_AmbientFactor << std::endl;
	std::cout << mu_LightDir << std::endl;
	std::cout << mu_LightColor << std::endl;

	glUseProgram(0);
}

void BasicShader::UseProgram() const
{
	glUseProgram(m_Program);
}

void BasicShader::UnUseProgram() const
{
	glUseProgram(0);
}

unsigned int BasicShader::Program() const
{
	return m_Program;
}

int BasicShader::u_TransformMat() const
{
	return mu_TransformMat;
}

int BasicShader::u_RotationMat() const
{
	return mu_RotationMat;
}

int BasicShader::u_AmbientFactor() const
{
	return mu_AmbientFactor;
}

int BasicShader::u_LightDir() const
{
	return mu_LightDir;
}

int BasicShader::u_LightColor() const
{
	return mu_LightColor;
}
