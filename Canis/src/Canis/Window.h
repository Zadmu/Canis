#pragma once

#include <string>
#include <memory>
#include <vector>

#include "Shader.h"

struct GLFWwindow;

class Window
{
public:
	Window(const std::string& title, uint32_t width, uint32_t height);
	~Window();

	void DrawTriangle(float x, float y);
	void DrawQuad2(float x, float y, float width, float height);
	void DrawQuad(float x, float y, float r, float g, float b, float a);
	void Update();

	float GetTime() const;
	float GetDeltaTime() const { return m_DeltaTime; }
	bool IsClosed() const;

	void A();
	void B();
	void C();
private:
	std::string m_Title;
	uint32_t m_Width, m_Height;
	float m_LastFrameTime = 0.0f;
	float m_DeltaTime = 0.0f;
	GLFWwindow* m_GLFWWindow;

	uint32_t m_VAO;
	uint32_t m_VertexBuffer, m_IndexBuffer;

	std::unique_ptr<Shader> m_Shader;
};