#pragma once

#include <string>

class Shader
{
public:
	Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
	~Shader();

	void Bind() const;

	uint32_t GetRendererID() const { return m_RendererID; }
private:
	uint32_t m_RendererID;
};
