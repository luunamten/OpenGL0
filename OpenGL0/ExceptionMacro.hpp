#pragma once
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <cassert>

#define LT_RUNTIME_ERR(message) \
	std::stringstream sstream;\
	sstream << __FILE__ << ", " << __LINE__ << ": " << message; \
	throw std::runtime_error(sstream.str())

#define LT_EXCEPTION(message) \
	std::stringstream sstream;\
	sstream << __FILE__ << ", " << __LINE__ << ": " << message; \
	throw std::exception(sstream.str())

#ifdef NDEBUG
#define LT_ASSERT(condition, message)
#else
#define LT_ASSERT(condition, message) \
				do { \
					std::cout << __FILE__ << ": " << __LINE__<< ": " << message << std::endl; \
					abort(); \
				} while(!condition)
#endif
