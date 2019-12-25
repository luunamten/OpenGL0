#pragma once
#include <sstream>
#include <iostream>
#include <stdexcept>

#define LT_RUNTIME_ERR(message) \
	std::stringstream sstream;\
	sstream << __FILE__ << ", " << __LINE__ << ": " << message; \
	throw std::runtime_error(sstream.str())

#define LT_EXCEPTION(message) \
	std::stringstream sstream;\
	sstream << __FILE__ << ", " << __LINE__ << ": " << message; \
	throw std::exception(sstream.str())