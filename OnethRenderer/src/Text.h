#pragma once

#include <iostream>
#include <map>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Shader.h"

struct Character {
	GLuint textureID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	GLuint advance;
};

class Text {
private:
	GLuint VAO, VBO;
	std::map<GLchar, Character> m_characters;
public:
	Text(const char* path, const int& size);
	void Draw(Shader& shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
};