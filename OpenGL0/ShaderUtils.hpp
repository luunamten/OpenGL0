#pragma once
#include <memory>

#define ATTR_COORD 0
#define ATTR_NORMAL 1

std::shared_ptr<char> GetShaderInfoLog(unsigned int shader);

std::shared_ptr<char> GetProgramInfoLog(unsigned int program);

std::shared_ptr<char> GetShaderCode(const char* path);