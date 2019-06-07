#pragma once

class Texture {
private:
	unsigned int m_rendererID;
public:
	Texture(const char* texturePath, unsigned int internalFormat, unsigned int imageFormat, bool flipVertically);
	Texture(const char* texturePath, unsigned int internalFormat, unsigned int imageFormat,
		unsigned int textureUnit, bool flipVertically);
	void SetTexParameteri(unsigned int target, unsigned int pname, int param);
	void SetTexParameterfv(unsigned int target, unsigned int pname, const float* params);

	void Bind() const;
	void UnBind() const;
};