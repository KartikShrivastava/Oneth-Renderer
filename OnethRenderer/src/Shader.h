#pragma once

class Shader {
private:
	unsigned int m_RendererID;
public:
	Shader(const char* vertexFilePath, const char* fragFilePath);

	unsigned int CompileShader(const char* filePath, GLenum type);
	std::string GetShaderCode(const char* path);
	bool CheckShaderErrors(const unsigned int& id, PFNGLGETSHADERIVPROC GetShaderiv, PFNGLGETSHADERINFOLOGPROC GetShaderInfoLog, GLenum query);
	void SetUniform4f(const char* uniformName, float v1, float v2, float v3, float v4);

	void Bind() const;
	void UnBind() const;
};