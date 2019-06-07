#include <iostream>
#include <glad/glad.h>

#include "Texture.h"
#include "expat/stb_image/stb_image.h"

Texture::Texture(const char * texturePath, unsigned int internalFormat, unsigned int imageFormat, bool flipVertically) {
	glGenTextures(1, &m_rendererID);
	glBindTexture(GL_TEXTURE_2D, m_rendererID);
	//GL_MIRRORED_REPEAT, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_EDGE, GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float bv[] = { 0.2f, 1.0f, 0.5f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bv);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	std::cout << texturePath[strlen(texturePath) - 1] << std::endl;

	stbi_set_flip_vertically_on_load(flipVertically);
	int width, height, nChannels;
	unsigned char* image = stbi_load(texturePath, &width, &height, &nChannels, 0);
	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Unable to load texture man!" << std::endl;
	}
	stbi_image_free(image);
}

Texture::Texture(const char * texturePath, unsigned int internalFormat, unsigned int imageFormat,
	unsigned int textureUnit, bool flipVertically){
	glGenTextures(1, &m_rendererID);
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_rendererID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float bv[] = { 0.2f, 1.0f, 0.5f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bv);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(flipVertically);
	int width, height, nChannels;
	unsigned char* image = stbi_load(texturePath, &width, &height, &nChannels, 0);
	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Unable to load texture man!" << std::endl;
	}
	stbi_image_free(image);
}

void Texture::SetTexParameteri(unsigned int target, unsigned int pname, int param){
	glTexParameteri(target, pname, param);
}

void Texture::SetTexParameterfv(unsigned int target, unsigned int pname, const float * params){
	glTexParameterfv(target, pname, params);
}

void Texture::Bind() const{
	glBindTexture(GL_TEXTURE_2D, m_rendererID);
}

void Texture::UnBind() const{
	glBindTexture(GL_TEXTURE_2D, 0);
}