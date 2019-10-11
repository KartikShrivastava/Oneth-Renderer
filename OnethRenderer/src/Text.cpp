#include "Text.h"
#include "CheckGLErrors.h"

Text::Text(const char* path, const int& size) {
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType library" << std::endl;

	FT_Face face;
	if (FT_New_Face(ft, path, 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font file" << std::endl;

	FT_Set_Pixel_Sizes(face, 0, size);

	GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

	for (GLubyte c = 0; c < 128; ++c) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
			continue;
		}
		GLuint texture;
		GLCall(glGenTextures(1, &texture));
		GLCall(glBindTexture(GL_TEXTURE_2D, texture));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		m_characters.insert(std::pair<GLchar, Character>(c, character));
	}
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glGenBuffers(1, &VBO));
	GLCall(glBindVertexArray(VAO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW));
	GLCall(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindVertexArray(0));
}

void Text::Draw(Shader& shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
	shader.Bind();

	GLCall(glActiveTexture(GL_TEXTURE0 + 10));
	GLCall(glBindVertexArray(VAO));

	std::string::const_iterator i;
	for (i = text.begin(); i != text.end(); i++) {
		Character ch = m_characters[*i];

		GLfloat xpos = x + ch.bearing.x * scale;
		GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;

		GLfloat w = ch.size.x * scale;
		GLfloat h = ch.size.y * scale;

		GLfloat verts[][4] = {
			xpos,	ypos + h,	0.0f,	0.0f,
			xpos,	ypos,	0.0f,	1.0f,
			xpos + w,	ypos,	1.0f,	1.0f,

			xpos,	ypos + h,	0.0f,	0.0f,
			xpos + w,	ypos,	1.0f,	1.0f,
			xpos + w,	ypos + h,	1.0f,	0.0f,
		};

		shader.SetUniform1i("u_text", 10);
		GLCall(glBindTexture(GL_TEXTURE_2D, ch.textureID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));

		x += (ch.advance >> 6)*scale;
	}
	GLCall(glBindVertexArray(0));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	shader.UnBind();
}