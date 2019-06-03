#include <iostream>
#include <sstream>
#include <glad/glad.h>

#include "CheckGLErrors.h"

void ClearErrors() {
	while (glGetError() != GL_NO_ERROR);
}

bool CheckErrors(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::stringstream ss;
		ss << std::hex << error;
		std::cout << "OPENGK error occoured : 0x" << ss.str() << "\ngl function : " << function << std::endl;
		return false;
	}
	return true;
}