#pragma once

#define ASSERT(x) if(!(x)) __debugbreak();

#ifdef NDEBUG
	#define GLCall(x) x
#else
	#define GLCall(x)   ClearErrors();\
						x;\
						ASSERT(CheckErrors(#x, __FILE__, __LINE__))
#endif

void ClearErrors();

bool CheckErrors(const char* function, const char* file, int line);